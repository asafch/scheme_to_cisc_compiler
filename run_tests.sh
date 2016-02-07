for f in tests/*.scm; do 
  echo "#use \"compiler.ml\";;
  Code_Gen.compile_scheme_file \"$f\" \"out_test.c\";;" | ocaml -stdin
  gcc -o out_test out_test.c
  our=`./out_test`
  petite=`cat $f | petite -q`
  result=""
  if [ "$our" = "$petite" ]; then
    result="#t"
  else
    result=`echo "(equal? '$our '$petite)" | petite -q`
  fi
  
  if [ "$result" = "#t" ]; then 
      echo "Test $f Passed"
  else
      echo "*** RESULTS DIFFER in $f"
      echo "*** scheme output: $petite"
      echo "*** our output: $our"
  fi

done