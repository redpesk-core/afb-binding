/*
 * Copyright (C) 2015-2026 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES ORCONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>

static const char SPACE = ' ';
static const char CROSS = 'X';
static const char ROUND = 'O';
static const char NHERE = '+';
static const int DEFLVL = 8;

/*
 * definition of a board
 */
struct board
{
	struct board *next;
	int use_count;
	int moves;
	int history[9];
	int id;
	int level;
	char board[9];
	afb_event_t event;
};

/*
 * list of boards
 */
static struct board *all_boards;

/*
 * Searchs a board having the 'id'.
 * Returns it if found or NULL otherwise.
 */
static struct board *search_board(int id)
{
	struct board *board = all_boards;
	while (board != NULL && board->id != id)
		board = board->next;
	return board;
}

/*
 * Creates a new board and returns it.
 */
static struct board *get_new_board(afb_req_t req)
{
	/* allocation */
	struct board *board = calloc(1, sizeof *board);

	/* initialisation */
	memset(board->board, SPACE, sizeof board->board);
	board->use_count = 1;
	board->level = DEFLVL;
	board->moves = 0;
	do {
		board->id = (rand() >> 2) % 1000;
	} while(board->id == 0 || search_board(board->id) != NULL);
	afb_api_new_event(afb_req_get_api(req), "board", &board->event);

	/* link */
	board->next = all_boards;
	all_boards = board;
	return board;
}

/*
 * Release a board
 */
static void release_board(struct board *board)
{
	/* decrease the reference count ... */
	if (--board->use_count == 0) {
		/* ... no more use */
		afb_event_unref(board->event);
		/* unlink from the list of boards */
		struct board **prv = &all_boards;
		while (*prv != NULL && *prv != board)
			prv = &(*prv)->next;
		if (*prv != NULL)
			*prv = board->next;
		/* release the used memory */
		free(board);
	}
}

/*
 * Checks who wins
 * Returns zero if there is no winner
 * Returns the char of the winner if a player won
 */
static char wins(const char b[9], int first, int incr)
{
	char c = b[first];

	return c != SPACE && b[first + incr] == c && b[first + incr + incr] == c ? c : 0;
}

/*
 * Checks who wins
 * Returns zero if there is no winner
 * Returns the char of the winner if a player won
 */
static char winner(const char b[9])
{
	char c;

	c = wins(b, 0, 1);
	if (c)
		return c;

	c = wins(b, 3, 1);
	if (c)
		return c;

	c = wins(b, 6, 1);
	if (c)
		return c;

	c = wins(b, 0, 3);
	if (c)
		return c;

	c = wins(b, 1, 3);
	if (c)
		return c;

	c = wins(b, 2, 3);
	if (c)
		return c;

	c = wins(b, 0, 4);
	if (c)
		return c;

	c = wins(b, 2, 2);
	if (c)
		return c;

	return 0;
}

/* get the color (X or 0) of the move of index 'move' */
static char color(int move)
{
	return (move & 1) == 0 ? CROSS : ROUND;
}

/* adds the move to the board */
static void add_move(struct board *board, int index)
{
	int imove = board->moves++;
	board->history[imove] = index;
	board->board[index] = color(imove);
}

/* get a random possible move index from the board described by 'b' */
static int get_random_move(char b[9])
{
	int i, index = rand() % 9;
	for (i = 0 ; i < 9 ; i++)
		if (b[index] != SPACE)
			index = (index + 1) % 9;
		else
			return index;
	for (i = 0 ; i < 9 ; i++)
		if (b[index] != NHERE)
			index = (index + 1) % 9;
		else
			return index;
	return index;
}

/*
 * Scores the position described by 'b'
 * for the player of color 'c' using an analysis of 'depth'.
 * Returns 1 if player 'c' wins.
 * Returns -1 if opponent of player 'c' wins.
 * returns 0 otherwise.
 */
static int score_position(char b[9], char c, int depth)
{
	int i, s, n, l;

	/* check if winning */
	if (winner(b) == c)
		return 1;

	/* when depth of analysis is reached return unknown case */
	if (--depth <= 0)
		return 0;

	/* switch to the opponent */
	c = (char)(ROUND + CROSS - c);

	/* inspect opponent moves */
	n = l = 0;
	for (i = 0 ; i < 9 ; i++) {
		if (b[i] == SPACE) {
			b[i] = c;
			s = score_position(b, c, depth);
			b[i] = SPACE;
			if (s > 0)
				return -1;
			n++;
			l += !!s;
		}
	}
	return n && n == l;
}

/* get one move: return the computed index of the move */
static int get_move(struct board *board)
{
	int index, depth, s;
	char c;
	char b[9];

	/* compute the depth */
	depth = board->level - 1;
	if (board->moves + depth > 9)
		depth = 9 - board->moves;

	/* case of null depth */
	if (depth == 0)
		return get_random_move(board->board);

	/* depth and more */
	memcpy(b, board->board, 9);
	c = color(board->moves);
	for (index = 0 ; index < 9 ; index++) {
		if (board->board[index] == SPACE) {
			board->board[index] = c;
			s = score_position(board->board, c, depth);
AFB_INFO("%d -> %d\n[%.3s|\n|%.3s|\n|%.3s]",index,s,board->board,board->board+3,board->board+6);
			board->board[index] = SPACE;
			if (s > 0)
				return index;
			if (s < 0)
				b[index] = NHERE;
		}
	}
	return get_random_move(b);
}

/*
 * get the board description
 */
static struct json_object *describe(struct board *board)
{
	int i;
	char w;
	struct json_object *resu, *arr;

	resu = json_object_new_object();

	json_object_object_add(resu, "boardid", json_object_new_int(board->id));
	json_object_object_add(resu, "level", json_object_new_int(board->level));

	arr = json_object_new_array();
	json_object_object_add(resu, "board", arr);
	for (i = 0 ; i < 9 ; i++)
		json_object_array_add(arr,
				json_object_new_string_len(&board->board[i], 1));

	arr = json_object_new_array();
	json_object_object_add(resu, "history", arr);
	for (i = 0 ; i < board->moves ; i++)
		json_object_array_add(arr, json_object_new_int(board->history[i]));

	w = winner(board->board);
	if (w)
		json_object_object_add(resu, "winner", json_object_new_string_len(&w, 1));
	else if (board->moves == 9)
		json_object_object_add(resu, "winner", json_object_new_string("none"));

	return resu;
}

/*
 * creates the data for a static string
 */
static inline afb_data_t static_data_string(const char *string)
{
	afb_data_t data;
	afb_create_data_raw(&data, AFB_PREDEFINED_TYPE_STRINGZ,
	                    string, 1 + strlen(string),
	                    NULL, NULL);
	return data;
}

static void req_fail(afb_req_t req, int code, const char *reason)
{
	afb_data_t data = static_data_string(reason);
	afb_req_reply(req, AFB_USER_ERRNO(code), 1, &data);
}

static void reply_description(afb_req_t req, struct board *board)
{
	afb_data_t data;
	struct json_object *description = describe(board);
	afb_create_data_raw(&data, AFB_PREDEFINED_TYPE_JSON_C,
	                    description, 0,
	                    (void*)json_object_put, description);
	afb_req_reply(req, 0, 1, &data);
}

/*
 * signals a change of the board
 */
static void changed(struct board *board, const char *reason)
{
	afb_data_t data = static_data_string(reason);
	afb_event_push(board->event, 1, &data);
}

static void release_board_cb(void *closure)
{
	struct board *board = closure;
	return release_board(board);
}

static int board_of_req_init_cb(void *closure, void **value, void (**freecb)(void*), void **freeclo)
{
	afb_req_t req = closure;
	struct board *board = get_new_board(req);

	*value = board;
	*freecb = release_board_cb;
	*freeclo = board;
	afb_req_subscribe(req, board->event);
}

/*
 * retrieves the board of the request
 */
static inline struct board *board_of_req(afb_req_t req)
{
	struct board *board;
	afb_req_context(req, (void**)&board, board_of_req_init_cb, req);
	return board;
}

/*
 * start a new game
 */
static void new(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	struct board *board;

	/* retrieves the context for the session */
	board = board_of_req(req);
	AFB_INFO("method 'new' called for boardid %d", board->id);

	/* reset the game */
	memset(board->board, SPACE, sizeof board->board);
	board->moves = 0;

	/* replies */
	afb_req_reply(req, 0, 0, NULL);

	/* signal change */
	changed(board, "new");
}

/*
 * get the board
 */
static void board(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	struct board *board;
	struct json_object *description;
	afb_data_t data;

	/* retrieves the context for the session */
	board = board_of_req(req);
	AFB_INFO("method 'board' called for boardid %d", board->id);

	reply_description(req, board);
}

/*
 * move a piece
 */
static void move(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	struct board *board;
	int i;
	afb_data_t data = NULL;

	/* retrieves the context for the session */
	board = board_of_req(req);
	AFB_INFO("method 'move' called for boardid %d", board->id);

	/* retrieves the arguments of the move */
	afb_req_param_convert(req, 0, AFB_PREDEFINED_TYPE_I32, &data);
	i = data == NULL ? -1 : (int)*(int32_t*)afb_data_ro_pointer(data);

	/* checks validity of arguments */
	if (i < 0 || i > 8) {
		AFB_WARNING("can't move to %s: %s", data?"wrong value":"not set");
		req_fail(req, 1, "bad request");
		return;
	}

	/* checks validity of the state */
	if (winner(board->board) != 0) {
		AFB_WARNING("can't move to %d: game is terminated", i);
		req_fail(req, 2, "game terminated");
		return;
	}

	/* checks validity of the move */
	if (board->board[i] != SPACE) {
		AFB_WARNING("can't move to %d: room occupied", i);
		req_fail(req, 3, "occupied");
		return;
	}

	/* applies the move */
	AFB_INFO("method 'move' for boardid %d, index=%d", board->id, i);
	add_move(board, i);

	/* replies */
	afb_req_reply(req, 0, 0, NULL);

	/* signals change */
	changed(board, "move");
}

/*
 * set the level
 */
static void level(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	struct board *board;
	int l;
	afb_data_t data = NULL;

	/* retrieves the context for the session */
	board = board_of_req(req);
	AFB_INFO("method 'level' called for boardid %d", board->id);

	/* retrieves the arguments */
	afb_req_param_convert(req, 0, AFB_PREDEFINED_TYPE_I32, &data);
	l = data == NULL ? -1 : (int)*(int32_t*)afb_data_ro_pointer(data);

	/* check validity of arguments */
	if (l < 1 || l > 10) {
		AFB_WARNING("can't set level: %s", data?"wrong value":"not set");
		req_fail(req, 1, "bad request");
		return;
	}

	/* set the level */
	AFB_INFO("method 'level' for boardid %d, level=%d", board->id, l);
	board->level = l;

	/* replies */
	afb_req_reply(req, 0, 0, NULL);

	/* signals change */
	changed(board, "level");
}

/*
 * Join a board
 */
static void join(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	struct board *board, *new_board;
	const char *id;
	afb_data_t data = NULL;

	/* retrieves the context for the session */
	board = board_of_req(req);
	AFB_INFO("method 'join' called for boardid %d", board->id);

	/* retrieves the arguments */
	afb_req_param_convert(req, 0, AFB_PREDEFINED_TYPE_STRINGZ, &data);
	if (data == NULL)
		goto bad_request;
	id = (const char*)afb_data_ro_pointer(data);

	/* none is a special id for joining a new session */
	if (strcmp(id, "none") == 0) {
		new_board = get_new_board(req);
		goto setctx;
	}

	/* searchs the board to join */
	new_board = search_board(atoi(id));
	if (new_board == NULL)
		goto bad_request;

	/*
	 * joining its board is stupid but possible
	 * however when called with the same stored pointer
	 * afb_req_context_set will not call the release
	 * function 'release_board'. So the use_count MUST not
	 * be incremented.
	 */
	if (new_board == board)
		goto success;

	new_board->use_count++;
setctx:
	/* set the new board (and leaves the previous one) */
	afb_req_unsubscribe(req, board->event);
	afb_req_context_set(req, new_board, release_board_cb, new_board);
	afb_req_subscribe(req, new_board->event);

success:
	/* replies */
	afb_req_reply(req, 0, 0, NULL);
	return;

bad_request:
	AFB_WARNING("can't join boardid %s: %s", id ? : "?", !id ? "no boardid" : atoi(id) ? "not found" : "bad boardid");
	req_fail(req, 1, "bad request");
}

/*
 * Undo the last move
 */
static void undo(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	struct board *board;
	int i;

	/* retrieves the context for the session */
	board = board_of_req(req);
	AFB_INFO("method 'undo' called for boardid %d", board->id);

	/* checks the state */
	if (board->moves == 0) {
		AFB_WARNING("can't undo");
		req_fail(req, 1, "bad request");
		return;
	}

	/* undo the last move */
	i = board->history[--board->moves];
	board->board[i] = SPACE;

	/* replies */
	afb_req_reply(req, 0, 0, NULL);

	/* signals change */
	changed(board, "undo");
}

/*
 * computer plays
 */
static void play(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	struct board *board;
	int index;

	/* retrieves the context for the session */
	board = board_of_req(req);
	AFB_INFO("method 'play' called for boardid %d", board->id);

	/* checks validity of the state */
	if (winner(board->board) != 0 || board->moves == 9) {
		AFB_WARNING("can't play: game terminated (%s)", winner(board->board) ? "has winner" : "no room left");
		req_fail(req, 1, "game terminated");
		return;
	}

	/* gets the move and plays it */
	index = get_move(board);
	add_move(board, index);

	/* replies */
	reply_description(req, board);

	/* signals change */
	changed(board, "play");
}

/*
 * array of the verbs exported to afb-daemon
 */
static const afb_verb_t verbs[] = {
   { .verb="new",   .callback=new },
   { .verb="play",  .callback=play },
   { .verb="move",  .callback=move },
   { .verb="board", .callback=board },
   { .verb="level", .callback=level },
   { .verb="join",  .callback=join },
   { .verb="undo",  .callback=undo },
   { .verb=NULL }
};

/*
 * description of the binding for afb-daemon
 */
const afb_binding_t afbBindingV4 = {
	.api = "tictactoe",
	.verbs = verbs,
	.noconcurrency = 1
};


