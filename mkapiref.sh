#!/bin/bash

# remove unexpected lines
clean-h-file() {
   sed /static.inline/d |
   sed /^__attribute__/d |
   sed '/^#if/d' |
   sed '/^#endif/d'
}

# extract groups, functions, predefined-types
delim-group-fun-typ() {
   awk '
	# declare a group
	$2=="@defgroup" { # ex: "/** @defgroup AFB_PREDEFINED_TYPE"
		group=tolower($3)
		ingrp=0
		print "BEGIN-GROUP " group
		next
	}
	# begin of a group
	!ingrp { # ex: " *  @{ */"
		if ($2=="@{")
			ingrp = group
		next
	}
	# end of a group
	ingrp && $2=="@}" { # ex: "/** @} */"
		print "END-GROUP " group
		ingrp = 0
		group = ""
		infun = ""
		next
	}
	# detect start of function
	!infun {
		if ($1=="/**")
			infun = $0
		next
	}
	# detect predefined types
	/#define *AFB_PREDEFINED_TYPE_/ {
		print "BEGIN-TYPE "$2
		print infun
		print "END-TYPE "$2
		infun = 0
		funame = 0
		next
	}
	# detect name of the function
	/^afb_.*\(/ {
		funame=$1
		sub(/\(.*/,"",funame)
	}
	# end of the function
	$1 == ")" {
		print "BEGIN-FUNCTION "funame
		print infun
		print ");"
		print "END-FUNCTION "funame
		infun = 0
		funame = 0
		next
	}
	# comment + head of function
	1 {
		infun = infun "\n" $0
	}'
}

delim-to-markdown() {
	LANG=C gawk '
	function esc(x) { # escapes the characters \ and *
		gsub("[_*]","\\\\&",x);
		return x;
	}
        function add(x,prefix) {
		if (file)
			idx["["esc(x)"]("file".html#"tolower(prefix x)")"] = 1
	}
	function tof(x, prefix) {
		gsub("_","-",x)
		return prefix x;
	}
	$1=="BEGIN-GROUP" && $2=="afb_predefined_type" {
		print "BEGIN-FILE /dev/null"
		file = "predefined-afb-type"
		print ""
		print "# Predefined types **"esc($2)"**"
		print ""
		next
	}
	$1=="BEGIN-GROUP" {
		file = tof($2,"func-")
		print "BEGIN-FILE " file
		print ""
		print "# Functions of class **"esc($2)"**"
		print ""
		next
	}
	$1=="END-GROUP" {
		file = 0
		next
	}
	$1=="BEGIN-FUNCTION" {
		add($2,"Function-")
		print ""
		print "## Function "esc($2)
		print ""
		print "```c"
		next
	}
	$1=="END-FUNCTION" {
		print "```"
		next
	}
	$1=="BEGIN-TYPE" {
		add($2,"")
		print ""
		print "## Type "esc($2)
		print ""
		print "```c"
		next
	}
	$1=="END-TYPE" {
		print "```"
		next
	}
	1
	END {
		print "BEGIN-FILE index"
		print ""
		print "# Index"
		print ""
		for (i in idx)
			print i "^" | "LANG=C sort -f | tr ^ \\\\n"
	}'
}

group-to-files() {
	awk -v prefix="${1:-docs/reference-v4/}" '
	$1=="BEGIN-FILE" {
		name = $2
		if (name != "/dev/null")
			name = prefix name ".md"
		next
	}
	name {
		print > name
	}'
}

cat afb/afb-binding-x4.h |
clean-h-file  |
delim-group-fun-typ |
delim-to-markdown |
group-to-files "docs/reference-v4/"

