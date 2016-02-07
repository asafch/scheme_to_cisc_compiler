(append '(1 2 3) '(4 5 6))                  ; (1 . (2 . (3 . (4 . (5 . (6 . ()))))))
(append '(1 2) '(3 4) '(5 6))               ; (1 . (2 . (3 . (4 . (5 . (6 . ()))))))
(append 'a)                                 ; a
(append '(1 2 3) '() '(1 2 . 3))            ; (1 . (2 . (3 . (1 . (2 . 3)))))

(define x '(1 2)) (define y '(3 4)) (define z (append x y)) (set-car! x '*) (set-car! y '$) z                                           ; (1 . (2 . ($ . (4 . ()))))

(append '(1) 2)
(apply (lambda (x y z) (list x y z)) "hello" "world" '("!"))     ; ("hello" . ("world" . ("!" . ())))
(apply (lambda x x) '())                    ; ()
(apply (lambda x x) 1 '(2))                 ; (1 . (2 . ()))
(apply (lambda x x) '(2))                   ; (2 . ())
(apply (lambda x x) 1 '())                  ; (1 . ())
(apply cons 1 '(2))                         ; (1 . 2)
(apply cons cons '(cons))                   ; (#<procedure cons> . cons)
(define fact-tail (lambda (n acc) (if (= 1 n) acc (fact-tail (- n 1) (* n acc))))) (apply fact-tail 5 '(1))                    ; 120
(define fact-tail (lambda (n acc) (if (= 1 n) acc (fact-tail (- n 1) (* n acc))))) (apply fact-tail '(5 1))                    ; 120
(define tail_test (lambda (n1) ((lambda (n2 n3) (+ n1 n3)) 10 15))) (tail_test 1)                               ; 16
(define tail_test (lambda (n1) ((lambda (n2 n3) (+ n1 n3)) 10 15))) (apply tail_test '(1))                      ; 16
(define tail_test (lambda (n1) ((lambda (n2 n3) (+ n1 n3)) 10 15))) (apply tail_test '(2))
(< 11)                                      ; #t
(< 1 2)                                     ; #t
(< -1/2 1/121)                              ; #t
(< 3 2)                                     ; #f
(< -3 -2 -1 0 1/2 2/3 3/4)
(= 1 2)                                     ; #f
(= 3 3 3 3 -1)                              ; #f
(= 5 5 5 5 5 5 5 5 5 5 5)                   ; #t
(= 2 4/2)
(> 11)                                      ; #t
(> 1 2)                                     ; #f
(> -1/2 1/121)                              ; #f
(> 3 2)                                     ; #t
(> -3 -2 -1 0 1/2 2/3 3/4)
(+)                                         ; 0
(+ 1 -1)                                    ; 0
(+ 3 -1/2)                                  ; 5/2
(+ 1/3 1/2)                                 ; 5/6
(+ 1/6 1/6)                                 ; 1/3
(+ 1/4 1/4 1/4 1/4)                         ; 1
(+ 1/2 1/3 1/4 1/5)                         ; 77/60
(+ 1/2 -1/3 1/4 -1/5)
(/ 1 3)                                     ; 1/3
(/ 4 2)                                     ; 2
(/ 6 -2)                                    ; -3
(/ 1)                                       ; 1
(/ 7)                                       ; 1/7
(/ 1 -12)
(*)                                         ; 1
(* 2)                                       ; 2
(* 5 2/7)                                   ; 10/7
(* -3 -3)                                   ; 9
(* -1/17 3/19)                                    ; exception - wrong number of arguments
(- 3)                                       ; -3
(- 4/7)                                     ; -4/7
(- 1 -1)                                    ; 2
(- 3 -1/2)                                  ; 7/2
(- 1/3 1/2)                                 ; -1/6
(- 1/6 1/6)                                 ; 0
(- 1/4 1/4 1/4 1/4)                         ; -1/2
(- 1/2 1/3 1/4 1/5)                         ; -17/60
(- 1/2 -1/3 1/4 -1/5)
(boolean? #t)                               ; #t
(boolean? #f)                               ; #t
(boolean? (lambda (x y z) x y z y x))       ; #f
(boolean? (if #f #f #f))                    ; #t
(boolean? (if #f #t))                       ; #f
(boolean? (and 1 2))                        ; #f
(boolean? (and #f #f))                      ; #t
(boolean? (and))                            ; #t
(boolean? 5)
(car '(1 2 3))
(cdr '(1 2 3))
(char->integer #\A)                         ; 65
(char->integer #\page)
(char? #\5)                                 ; #t
(char? #\newline)                           ; #t
(char? -1/19)
(cons (lambda () #t) (if #f #t))            ; (#<procedure at...> . #<void>)
(cons 0xabc #t)                             ; (2748 . #t)
(cons 1 (cons -2 (cons 3 (cons #t '()))))
(denominator -2)                            ; 1
(denominator 9/7)
(eq? (if #f #f) (if #f #f))                 ; #t
(eq? '() '())                               ; #t
(eq? #f #f)                                 ; #t
(eq? #f #t)                                 ; #f
(eq? #\a #\A)                               ; #f
(eq? #\newline #\newline)                   ; #t
(eq? -1 -1)                                 ; #t
(eq? -1 2)                                  ; #f
(eq? 1/3 2/6)                               ; #t
(eq? 1/3 1/4)                               ; #f
(eq? "ab" "ab")                             ; #t (because of constants table)
(eq? 'ab 'ab)                               ; #t
(eq? 'e 'q)                                 ; #f
(eq? '(1 2) '(1 2))                         ; #t
(eq? '(1 2) '(1))                           ; #f
(eq? '#(vector) '#(vector))                 ; #t
(eq? '#(vector) '#(list))
(integer? -10000000)                        ; #t
(integer? 3/14)                             ; #f
(integer? cons)
(integer->char 10)                          ; #\newline
(integer->char 255)
(list list)                                 ; (#<procedure at ...> . ())
(list -1 2 -3 4)                            ; (-1 . (2 . (-3 . (4 . ()))))
(list #t #t #f #t)                          ; (#t . (#t . (#f . (#t . ()))))
(list)
(make-string 3)                             ; "\000;\000;\000;"
(make-string 5 #\r)
(make-vector 7)                             ; #(0 0 0 0 0 0 0)
(make-vector 2 "scheme")
(map (lambda (s) "batman") '("why" "so" "serious?"))
(map (lambda (x) x) '(-1 -2 -3/5))          ; (-1 . (-2 . (-3/5 . ())))
(map + '(1 2 1/3) '(-1 -2 -1/3))            ; (0 . (0 . (0 . ())))
(map list '("a" "b" "c") '('a 'b 'c) '(1 2 3) '(1/2 1/3 1/4))
(not #f)                                    ; #t
(not #t)                                    ; #f
(not 5)                                     ; #f
(not apply)
(null? '())                                 ; #t
(null? '(9 #\F))                            ; #f
(null? null?)                               ; #f
(number? -1)                                ; #t
(number? -3/14)                             ; #t
(number? #t)                                ; #f
(number? "hello, world!")
(numerator 4/2)                             ; 2
(numerator -7/17)                           ; -7
(numerator 3)
(pair? '(1 2))                              ; #t
(pair? '())                                 ; excpetion - wrong number of arguments
(pair? 5)
(procedure? procedure?)                     ; #t
(procedure? (lambda s (car s)))             ; #t
(procedure? #t)
(rational? 5/2)                             ; #t
(rational? -99)                             ; #t
(rational? 0xffff)                          ; #t
(rational? "rational?")
(remainder 12 -5)                           ; 2
(remainder 70 7)                            ; 0
(remainder 0xffff 0xfffe)
(set-car! '(a b c) '(x y z))
(set-car! '(1 2 3) (cdr '(1 2 3)))
(define l '(1 2 3)) (set-car! l '(4 5)) l (car l) (cdr l)
(set-cdr! '(a b c) '(x y z))
(set-cdr! '(1 2 3) (cdr '(1 2 3)))
(define l '(1 2 3)) (set-cdr! l '(4 5)) l (car l) (cdr l)
(string-length "")                          ; 0
(string-length "a")
(string-ref "string" 1)
(string-set! "string" 1 #\p)
(string->symbol "sym")                      ; sym
(string->symbol "freshwithsymbolsinprogram")   ; freshwithsymbolsinprogram
(eq? (string->symbol (make-string 1 #\a)) 'a)        ; strings should be comparisoned deeply when examining the symbol's string representation
(define str (symbol->string 'test)) (eq? (string->symbol str) 'test)            ; #t
(string->symbol "fresh")
(string? "string")                          ; #t
(string? 'string)                           ; #f
(string? 2)
(symbol? 'symbol)                           ; #t
(symbol? "symbol")                          ; #f
(symbol? 2)
(symbol->string 'sym)
(vector vector vector)                      ; #(#<procedure...> #<procedure...>)
(vector 'a 1 1/1 "uno" 'einz)               ; #(a 1 1 "uno" einz)
(vector)
(vector-length '#())                        ; 0
(vector-length '#(vector? vector-ref))
(vector-ref '#(1 2 3) 0)
(vector-set! '#(4 5 6) 1 7)
(vector? "no")                              ; #f
(vector? '#(1 b 3 d 5 f))
(zero? 0)                                   ; #t
(zero? 0/3)                                 ; #t
(zero? (- 5 (+ 1 2 2)))
