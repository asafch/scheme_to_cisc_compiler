;append (variadic)
#;(append '(1 2 3) '(4 5 6))                  ; (1 . (2 . (3 . (4 . (5 . (6 . ()))))))
#;(append 'a)                                 ; exception - wrong number of arguments
#;(append 2 1)                                ; exception - l1 not a list
#;(append '(1) 2)                             ; exception - l2 not a list

;apply


;< (variadic)
#;(< 1 2)                                     ; #t
#;(< 3 2)                                     ; #f
#;(< -3 -2 -1 0 1/2 2/3 3/4)                  ; #t
#;(<)                                         ; exception - wrong number of arguments
#;(< 'a 6)                                    ; exception - 'a is not a number

;= (variadic)
#;(= 1 2)                                     ; #f
#;(= 3 3 3 3 -1)                              ; #f
#;(= 5 5 5 5 5 5 5 5 5 5 5)                   ; #t
#;(=)                                         ; exception - wrong number of arguments
#;(= 1 '#(1))                                 ; exception - #(1) is not a number

;> (variadic)
#;(> 1 2)                                     ; #f
#;(> 3 2)                                     ; #t
#;(> -3 -2 -1 0 1/2 2/3 3/4)                  ; #f
#;(>)                                         ; exception - wrong number of arguments
#;(> 'a 6)                                    ; exception - 'a is not a number

;+ (variadic)
#;(+)                                         ; 0
#;(+ 1 -1)                                    ; 0
#;(+ 3 -1/2)                                  ; 5/2
#;(+ 5 'a)                                    ; exception - 'a is not a number
#;(+ 1/3 1/2)                                 ; 5/6

;/ (variadic)
#;(/ 1 3)                                     ; 1/3
#;(/ 4 2)                                     ; 2
#;(/ 6 -2)                                    ; -3
#;(/)                                         ; exception - wrong number of arguments
#;(/ 1)                                       ; 1
#;(/ 5 #t)                                    ; exception - #t is not a number

;* (variadic)
#;(*)                                         ; 1
#;(* 5 2/7)                                   ; 10/7
#;(* -1 't)                                   ; excpetion - t is not a number

;boolean?
#;(boolean? #t)                               ; #t
#;(boolean? #f)                               ; #t
#;(boolean? (lambda (x y z) x y z y x))       ; #f
#;(boolean? (if #f #f #f))                    ; #t
#;(boolean? (if #f #t))                       ; #f
#;(boolean? (and 1 2))                        ; #t
#;(boolean? (and #f #f))                      ; #t
#;(boolean? (and))                            ; #t
#;(boolean?)                                  ; excpetion - wrong number of arguments
#;(boolean? 5)                                ; #f

;car
#;(car '(1 2 3))                              ; 1
#;(car 5)                                     ; exception - not a pair
#;(car)                                       ; exception - wrong number of arguments
#;(car '(1 2 3) 'a)                           ; exception - wrong number of arguments

;cdr
#;(cdr '(1 2 3))                              ; (2 . (3 . ()))
#;(cdr 5)                                     ; exception - not a pair
#;(cdr)                                       ; exception - wrong number of arguments
#;(cdr '(1 2 3) 'a)                           ; exception - wrong number of arguments

;char->integer
#;(char->integer #\A)                         ; 65
#;(char->integer #\page)                      ; 12
#;(char->integer 0)                           ; exception - 0 is not a char
#;(char->integer)                             ; excpetion - wrong number of arguments

;char?
#;(char? #\5)                                 ; #t
#;(char? #\newline)                           ; #t
#;(char? -1/19)                               ; #f
#;(char?)                                     ; excpetion - wrong number of arguments

;cons
#;(cons 0xabc #t)                             ; (2748 . #t)
#;(cons)                                      ; excpetion - wrong number of arguments
#;(cons #f)                                   ; excpetion - wrong number of arguments
#;(cons (lambda () #t) (if #f #t))            ; (#<procedure at...> . #<void>)

;denominator
#;(denominator -2)                            ; 1
#;(denominator 9/7)                           ; 7
#;(denominator)                               ; excpetion - wrong number of arguments
#;(denominator #\t)                           ; exception - #\t is not a rational number

;eq?
#;(eq? "ab" "ab")                             ; #t (because of constants table)
#;(eq? 'ab 'ab)                               ; #t
#;(eq? #f #f)                                 ; #t
#;(eq? -1 -1)                                 ; #t

;integer?
#;(integer? -10000000)                        ; #t
#;(integer? 3/14)                             ; #f
#;(integer? cons)                             ; #f
#;(integer?)                                  ; excpetion - wrong number of arguments

;integer->char
#;(integer->char 10)                          ; #\newline
#;(integer->char 255)                         ; #\ÿ
#;(integer->char '())                         ; exception - () is not a scalar value
#;(intger->char)                              ; excpetion - wrong number of arguments

;list (variadic)
#;(list list)                                 ; (#<procedure at ...> . ())
#;(list -1 2 -3 4)                            ; (-1 . (2 . (-3 . (4 . ()))))
#;(list #t #t #f #t)                          ; (#t . (#t . (#f . (#t . ()))))
#;(list)                                      ; ()

;make-string
#;(make-string 3)                             ; "\x0;\x0;\x0;"
#;(make-string 5 #\r)                         ; "rrrrr"
#;(make-string #f)                            ; excpetion - #f is not a number
#;(make-string)                               ; excpetion - wrong number of arguments

;make-vector
#;(make-vector 7)                             ; #(0 0 0 0 0 0 0)
#;(make-vector 2 "scheme")                    ; #("scheme" "scheme")
#;(make-vector "e")                           ; excpetion - e is not a number
#;(make-vector)                               ; excpetion - wrong number of arguments

;map
#;(map (lambda (s) "batman") '("why" "so" "serious?"))  ; ("batman" . ("batman" . ("batman" . ())))
#;(map #f '(1 cons list 'e))                  ; exception - #f is not a procedure
#;(map (lambda (x) x) '(-1 -2 -3/5))          ; (-1 . (-2 . (-3/5 . ())))
#;(map (lambda (k) k))                        ; excpetion - wrong number of arguments

;not
#;(not #f)                                    ; #t
#;(not #t)                                    ; #f
#;(not 5)                                     ; #f
#;(not)                                       ; excpetion - wrong number of arguments
#;(not apply)                                 ; #f

;null?
#;(null? '())                                 ; #t
#;(null? '(9 #\F))                            ; #f
#;(null?)                                     ; excpetion - wrong number of arguments
#;(null? null?)                               ; #f

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
