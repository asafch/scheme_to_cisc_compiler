;append (variadic)


;apply


;< (variadic)


;= (variadic)


;> (variadic)


;+ (variadic)


;/ (variadic)


;* (variadic)


;boolean?


;car
#;(car '(1 2 3))                            ; returns 1
#;(car 5)                                   ; exception - not a pair
#;(car)                                     ; exception - wrong number of arguments
#;(car '(1 2 3) 'a)                         ; exception - wrong number of arguments

;cdr
#;(cdr '(1 2 3))                            ; returns (2 . (3 . ()))
#;(cdr 5)                                   ; exception - not a pair
#;(cdr)                                     ; exception - wrong number of arguments
#;(cdr '(1 2 3) 'a)                         ; exception - wrong number of arguments

;char->integer


;char?


;cons


;denominator


;eq?


;integer?


;integer->char


;list (variadic)


;make-string


;make-vector


;map


;not


;null?


;number?


;numerator


;pair?
#;(pair? '(1 2))
#;(pair? '())
#;(pair?)
#;(pair? 5)
#;(pair? '(1 2) '(9))

;procedure?


;rational?


;remainder


;set-car!


;set-cdr!


;string-length


;string-ref


;string-set!


;string->symbol


;string?


;symbol?


;symbol->string


;vector


;vector-length


;vector-ref


;vector-set!


;vector?


;zero?
