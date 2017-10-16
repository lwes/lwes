AC_DEFUN([AX_PROG_BISON_VERSION], [
  AC_REQUIRE([AC_PROG_YACC])
  AC_REQUIRE([AC_PROG_EGREP])

  AC_CACHE_CHECK([if bison ($YACC, via $EGREP) is version 3.0 or newer],[ax_bison_new],[
    dnl  note m4 eats square brackets, so we have to use @<:@ and @:>@
    BISON_VERSION=`$YACC --version 2>/dev/null | $EGREP -o '@<:@0-9@:>@\.@<:@0-9@:>@+(\.@<:@0-9@:>@+)?'`
    BISON_MAJOR=`$YACC --version 2>/dev/null | $EGREP -o '@<:@0-9@:>@\.@<:@0-9@:>@+(\.@<:@0-9@:>@+)?' | $EGREP -o '^@<:@0-9@:>@+'`
    AC_SUBST(BISON_VERSION)
    AC_SUBST(BISON_MAJOR)
  ])
])
