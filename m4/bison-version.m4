AC_DEFUN([AX_PROG_BISON_NEW], [
  AC_REQUIRE([AC_PROG_YACC])
  AC_REQUIRE([AC_PROG_EGREP])

  AC_CACHE_CHECK([if bison ($YACC, via $EGREP) is version 3.0 or newer],[ax_bison_new],[
    dnl  note m4 eats square brackets, so we have to use @<:@ and @:>@
    BISON_VERSION=`$YACC --version 2>/dev/null | $EGREP -o '@<:@0-9@:>@\.@<:@0-9@:>@+\.@<:@0-9@:>@+'`
    BISON_MAJOR=`$YACC --version 2>/dev/null | $EGREP -o '@<:@0-9@:>@\.@<:@0-9@:>@+\.@<:@0-9@:>@+' | $EGREP -o '^@<:@0-9@:>@+'`
    AC_SUBST(BISON_VERSION)
    AC_SUBST(BISON_MAJOR)
    dnl tried lots of other options, including AS_VERSION_COMPARE, but they all failed for wonky reasons
    AS_IF(["$BISON_MAJOR"] 
            [test "$BISON_MAJOR" = "0"], [ax_bison_new=no], 
            [test "$BISON_MAJOR" = "1"], [ax_bison_new=no], 
            [test "$BISON_MAJOR" = "2"], [ax_bison_new=no], 
            [ax_bison_new=yes])
  ])
  AS_IF([test "$ax_bison_new" = "yes"], [
      dnl bison 3.0 and newer adds some additional options
      dnl like passing an implicit parameter to lexer/parser functions
      BISON_NEW="1"
      AC_SUBST(BISON_NEW)
    ] m4_ifnblank([$1], [[$1]]),
    m4_ifnblank([$2], [[$2]])
  )
])
