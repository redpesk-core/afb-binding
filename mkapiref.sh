#!/bin/bash

cat afb/afb-binding-x4.h |
sed /static.inline/d |
sed /^__attribute__/d |
awk '
$2=="@defgroup" {
	group=tolower($3)
	ingrp=0
	print "BEGIN-GROUP " group
	next
}
!ingrp {
	if ($2=="@{")
		ingrp = group
	next
}
ingrp && $2=="@}" {
	print "END-GROUP " group
	ingrp = 0
	group = ""
	infun = ""
	next
}
!infun {
	if ($1=="/**")
		infun = $0
	next
}
$1 == ")" {
	print "BEGIN-FUNCTION "funame
	print infun
	print ");"
	print "END-FUNCTION "funame
	infun = 0
	funame = 0
	next
}
/^afb_.*\(/ {
	funame=$1
	sub(/\(.*/,"",funame)
}
1 {
	infun = infun "\n" $0
}
' |
awk '
$1=="BEGIN-GROUP" {
	print
	print ""
	print "# Functions of class **"$2"**"
	print ""
	next
}
$1=="END-GROUP" {
	next
}
$1=="BEGIN-FUNCTION" {
	print ""
	print "## "$2
	print ""
	print "```C"
	next
}
$1=="END-FUNCTION" {
	print "```"
	next
}
1
' |
awk '
$1=="BEGIN-GROUP" {
	o = $2
	gsub("_","-",o)
	o = "docs/reference-v4/func-" o ".md"
	next
}
o {
	print >> o
}
'
