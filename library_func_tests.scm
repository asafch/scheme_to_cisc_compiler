;append (variadic)
;(append '(1 2 3) '(4 5 6))                  ; (1 . (2 . (3 . (4 . (5 . (6 . ()))))))
;(append 'a)                                 ; exception - wrong number of arguments
;(append 2 1)                                ; exception - l1 not a list
;(append '(1) 2)                             ; (1 . 2)

;apply
;(apply (lambda (x) x) '("application"))     ; "application"
;(apply cons 1 '(2))                         ; (1 . 2)
;(apply cons cons '(cons))                   ; (#<procedure cons> . cons)
;(define fact-tail
;    (lambda (n acc)
;      (if (= 1 n)
;          acc
;          (fact-tail (- n 1) (* n acc)))))
;(apply fact-tail 5 '(1))                    ; 120
;(apply cons 1 2)                            ; exception - not a proper list
;(apply)                                     ; exception - wrong number of arguments
;(apply cons)                                ; exception - wrong number of arguments

;< (variadic)
;(< 1 2)                                     ; #t
;(< 3 2)                                     ; #f
;(< -3 -2 -1 0 1/2 2/3 3/4)                  ; #t
;(<)                                         ; exception - wrong number of arguments
;(< 'a 6)                                    ; exception - 'a is not a number

;= (variadic)
;(= 1 2)                                     ; #f
;(= 3 3 3 3 -1)                              ; #f
;(= 5 5 5 5 5 5 5 5 5 5 5)                   ; #t
;(=)                                         ; exception - wrong number of arguments
;(= 1 '#(1))                                 ; exception - #(1) is not a number

;> (variadic)
;(> 1 2)                                     ; #f
;(> 3 2)                                     ; #t
;(> -3 -2 -1 0 1/2 2/3 3/4)                  ; #f
;(>)                                         ; exception - wrong number of arguments
;(> 'a 6)                                    ; exception - 'a is not a number

;+ (variadic)
;(+)                                         ; 0
;(+ 1 -1)                                    ; 0
;(+ 3 -1/2)                                  ; 5/2
;(+ 5 'a)                                    ; exception - 'a is not a number
;(+ 1/3 1/2)                                 ; 5/6

;/ (variadic)
;(/ 1 3)                                     ; 1/3
;(/ 4 2)                                     ; 2
;(/ 6 -2)                                    ; -3
;(/)                                         ; exception - wrong number of arguments
;(/ 1)                                       ; 1
;(/ 5 #t)                                    ; exception - #t is not a number

;* (variadic)
;(*)                                         ; 1
;(* 5 2/7)                                   ; 10/7
;(* -1 't)                                   ; excpetion - t is not a number

;boolean?
;(boolean? #t)                               ; #t
;(boolean? #f)                               ; #t
;(boolean? (lambda (x y z) x y z y x))       ; #f
;(boolean? (if #f #f #f))                    ; #t
;(boolean? (if #f #t))                       ; #f
;(boolean? (and 1 2))                        ; #f
;(boolean? (and #f #f))                      ; #t
;(boolean? (and))                            ; #t
;(boolean?)                                  ; excpetion - wrong number of arguments
;(boolean? 5)                                ; #f

;car
;(car '(1 2 3))                              ; 1
;(car 5)                                     ; exception - not a pair
;(car)                                       ; exception - wrong number of arguments
;(car '(1 2 3) 'a)                           ; exception - wrong number of arguments

;cdr
;(cdr '(1 2 3))                              ; (2 . (3 . ()))
;(cdr 5)                                     ; exception - not a pair
;(cdr)                                       ; exception - wrong number of arguments
;(cdr '(1 2 3) 'a)                           ; exception - wrong number of arguments

;char->integer
;(char->integer #\A)                         ; 65
;(char->integer #\page)                      ; 12
;(char->integer 0)                           ; exception - 0 is not a char
;(char->integer)                             ; excpetion - wrong number of arguments

;char?
;(char? #\5)                                 ; #t
;(char? #\newline)                           ; #t
;(char? -1/19)                               ; #f
;(char?)                                     ; excpetion - wrong number of arguments

;cons
;(cons (lambda () #t) (if #f #t))            ; (#<procedure at...> . #<void>)
;(cons 0xabc #t)                             ; (2748 . #t)
;(cons 1 (cons -2 (cons 3 (cons #t '()))))   ; (1 . (-2 . (3 . (#t . ()))))
;(cons)                                      ; excpetion - wrong number of arguments
;(cons #f)                                   ; excpetion - wrong number of arguments

;denominator
;(denominator -2)                            ; 1
;(denominator 9/7)                           ; 7
;(denominator)                               ; excpetion - wrong number of arguments
;(denominator #\t)                           ; exception - #\t is not a rational number

;eq?
;(eq? (if #f #f) (if #f #f))                 ; #t
;(eq? '() '())                               ; #t
;(eq? #f #f)                                 ; #t
;(eq? #f #t)                                 ; #f
;(eq? #\a #\A)                               ; #f
;(eq? #\newline #\newline)                   ; #t
;(eq? -1 -1)                                 ; #t
;(eq? -1 2)                                  ; #f
;(eq? 1/3 2/6)                               ; #t
;(eq? 1/3 1/4)                               ; #f
;(eq? "ab" "ab")                             ; #t (because of constants table)
;(eq? 'ab 'ab)                               ; #t
;(eq? 'e 'q)                                 ; #f
;(eq? '(1 2) '(1 2))                         ; #t
;(eq? '(1 2) '(1))                           ; #f
;(eq? '#(vector) '#(vector))                 ; #t
;(eq? '#(vector) '#(list))                   ; #f
;(eq?)                                       ; excpetion - wrong number of arguments
;(eq? 1 2 3)                                 ; excpetion - wrong number of arguments

;integer?
;(integer? -10000000)                        ; #t
;(integer? 3/14)                             ; #f
;(integer? cons)                             ; #f
;(integer?)                                  ; excpetion - wrong number of arguments

;integer->char
;(integer->char 10)                          ; #\newline
;(integer->char 255)                         ; y with two dots above it
;(integer->char '())                         ; exception - () is not a scalar value
;(intger->char)                              ; excpetion - wrong number of arguments

;list (variadic)
;(list list)                                 ; (#<procedure at ...> . ())
;(list -1 2 -3 4)                            ; (-1 . (2 . (-3 . (4 . ()))))
;(list #t #t #f #t)                          ; (#t . (#t . (#f . (#t . ()))))
;(list)                                      ; ()

;make-string
;(make-string 3)                             ; "\x0;\x0;\x0;"
;(make-string 5 #\r)                         ; "rrrrr"
;(make-string #f)                            ; excpetion - #f is not a number
;(make-string)                               ; excpetion - wrong number of arguments

;make-vector
;(make-vector 7)                             ; #(0 0 0 0 0 0 0)
;(make-vector 2 "scheme")                    ; #("scheme" "scheme")
;(make-vector "e")                           ; excpetion - e is not a number
;(make-vector)                               ; excpetion - wrong number of arguments

;map
;(map (lambda (s) "batman") '("why" "so" "serious?"))  ; ("batman" . ("batman" . ("batman" . ())))
;(map #f '(1 cons list 'e))                  ; exception - #f is not a procedure
;(map (lambda (x) x) '(-1 -2 -3/5))          ; (-1 . (-2 . (-3/5 . ())))
;(map (lambda (k) k))                        ; excpetion - wrong number of arguments

;not
;(not #f)                                    ; #t
;(not #t)                                    ; #f
;(not 5)                                     ; #f
;(not)                                       ; excpetion - wrong number of arguments
;(not apply)                                 ; #f

;null?
;(null? '())                                 ; #t
;(null? '(9 #\F))                            ; #f
;(null?)                                     ; excpetion - wrong number of arguments
;(null? null?)                               ; #f

;number?
;(number? -1)                                ; #t
;(number? -3/14)                             ; #t
;(number? #t)                                ; #f
;(number? "hello, world!")                    ; #f
;(number?)                                   ; excpetion - wrong number of arguments
;(number? 1 2)                               ; excpetion - wrong number of arguments

;numerator
;(numerator 4/2)                             ; 2
;(numerator -7/17)                           ; -7
;(numerator 3)                               ; 3
;(numerator 'const)                          ; exception - not a fraction
;(numerator)                                 ; excpetion - wrong number of arguments
;(numerator 1/2 1/3)                         ; excpetion - wrong number of arguments

;pair?
;(pair? '(1 2))                              ; #t
;(pair? '())                                 ; #f
;(pair?)                                     ; excpetion - wrong number of arguments
;(pair? 5)                                   ; #f
;(pair? '(1 2) '(9))                         ; excpetion - wrong number of arguments

;procedure?
;(procedure? procedure?)                     ; #t
;(procedure? (lambda s (car s)))             ; #t
;(procedure? #t)                             ; #f
;(procedure?)                                ; excpetion - wrong number of arguments
;(procedure? cons car cdr list)              ; excpetion - wrong number of arguments

;rational?
;(rational? 5/2)                             ; #t
;(rational? -99)                             ; #t
;(rational? 0xffff)                          ; #t
;(rational? "rational?")                     ; #f
;(rational?)                                 ; excpetion - wrong number of arguments
;(rational? 1/4 1/7)                         ; excpetion - wrong number of arguments

;remainder
;(remainder 12 -5)                           ; 2
;(remainder 70 7)                            ; 0
;(remainder 0xffff 0xfffe)                   ; 1
;(remainder 2 "cookies")                     ; exception - not an integer
;(remainder 2 1/2)                           ; exception - not an integer
;(remainder)                                 ; excpetion - wrong number of arguments
;(remainder 1 2 3)                           ; excpetion - wrong number of arguments

;set-car!
;(set-car! '(a b c) '(x y z))                ; #<void>, print nothing
;(set-car! '() 5)                            ; exception - not a pair
;(set-car! "set-car!" set-car!)              ; exception - not a pair
;(set-car! '(1 2 3) (cdr '(1 2 3)))          ; #<void>, print nothing
;(set-car!)                                  ; excpetion - wrong number of arguments

;set-cdr!
;(set-cdr! '(a b c) '(x y z))                ; #<void>, print nothing
;(set-cdr! '() 5)                            ; exception - not a pair
;(set-cdr! "set-cdr!" sdt-car!)              ; exception - not a pair
;(set-cdr! '(1 2 3) (cdr '(1 2 3)))          ; #<void>, print nothing
;(set-cdr!)                                  ; excpetion - wrong number of arguments

;string-length
;(string-length "")                          ; 0
;(string-length "a")                         ; 1
;(string-length 5)                           ; exception - not a string
;(string-length)                             ; excpetion - wrong number of arguments
;(string-length "a" "a")                     ; excpetion - wrong number of arguments

;string-ref
;(string-ref "string" 1)                     ; #\t
;(string-ref 'string 1)                      ; exception - not a string
;(string-ref "string" 99)                    ; exception - out of bounds
;(string-ref "string" -1)                    ; exception - out of bounds
;(string-ref "string" 'a)                    ; exception - not an integer
;(string-ref)                                ; excpetion - wrong number of arguments
;(string-ref "string")                       ; excpetion - wrong number of arguments
;(string-ref "string" 1 2)                   ; excpetion - wrong number of arguments

;string-set!
;(string-set! "string" 1 #\p)                ; #<void>
;(string-set! "string" 95 #\o)               ; exception - out of bounds
;(string-set! "string" -1 #\o)               ; exception - out of bounds
;(string-set! "string" 1 "kk")               ; exception - not a char
;(string-set! 'string 1 #\p)                 ; exception - not a string
;(string-set! "string" 1/2 #\p)              ; exception - not an integer
;(string-set!)                               ; excpetion - wrong number of arguments
;(string-set! "wstring" 1)                   ; excpetion - wrong number of arguments
;(string-set! "wwstring" 0 #\r #\l)          ; excpetion - wrong number of arguments

;string->symbol
;(string->symbol 'sym)                       ; exception - not a string
;(string->symbol "sym")                      ; sym
;(string->symbol)                            ; excpetion - wrong number of arguments
;(string->symbol "str" "sym")                ; excpetion - wrong number of arguments

;string?
;(string? "string")                          ; #t
;(string? 'string)                           ; #f
;(string? 2)                                 ; #f
;(string? "yes" "no")                        ; excpetion - wrong number of arguments
;(string?)                                   ; excpetion - wrong number of arguments

;symbol?
;(symbol? 'symbol)                           ; #t
;(symbol? "symbol")                          ; #f
;(symbol? 2)                                 ; #f
;(symbol? 'yes 'no)                          ; excpetion - wrong number of arguments
;(symbol?)                                   ; excpetion - wrong number of arguments

;symbol->string
;(symbol->string 'sym)                       ; "sym"
;(symbol->string "sym")                      ; exception - not a symbol
;(symbol->string)                            ; excpetion - wrong number of arguments
;(symbol->string 'a 'b)                      ; excpetion - wrong number of arguments

;vector
;(vector vector vector)                      ; #(#<procedure...> #<procedure...>)
;(vector 'a 1 1/1 "uno" 'einz)               ; #(a 1 1 "uno" einz)
;(vector)                                    ; #()

;vector-length
;(vector-length '#())                        ; 0
;(vector-length '#(vector? vector-ref))      ; 2
;(vector-length '#(1) '#(2))                 ; excpetion - wrong number of arguments
;(vector-length)                             ; excpetion - wrong number of arguments
;(vector-length '(v v v))                    ; exception - not a vector

;vector-ref
;(vector-ref '#(1 2 3) 0)                    ; 1
;(vector-ref '#(1 2 3) 3)                    ; exception - out of bounds
;(vector-ref '#(1 2 3))                      ; excpetion - wrong number of arguments
;(vector-ref '#(1 2 3) 0 1)                  ; excpetion - wrong number of arguments
;(vector-ref "'#(1 2 3)" 0)                  ; exception - not a vector

;vector-set!
;(vector-set! '#(4 5 6) 1 7)                 ; #<void>
;(vector-set! '#(4 5 6) 3 7)                 ; exception - out of bounds
;(vector-set! '#(4 5 6) -1 7)                ; exception - out of bounds
;(vector-set! "vector" 1 2)                  ; exception - not a vector
;(vector-set!)                               ; excpetion - wrong number of arguments
;(vector-set! '#(4 5 6) 1)                   ; excpetion - wrong number of arguments
;(vector-set! '#(4 5 6) 1 7 8)               ; excpetion - wrong number of arguments

;vector?
;(vector? "no")                              ; #f
;(vector? '#(1 b 3 d 5 f))                   ; #t
;(vector?)                                   ; excpetion - wrong number of arguments
;(vector? 1 2)                               ; excpetion - wrong number of arguments

;zero?
;(zero? 0)                                   ; #t
;(zero? 0/3)                                 ; #t
;(zero? #f)                                  ; #f
;(zero? (- 5 (+ 1 2 2)))                    ; #t
;(zero?)                                     ; excpetion - wrong number of arguments
;(zero? 0 0)                                 ; excpetion - wrong number of arguments
