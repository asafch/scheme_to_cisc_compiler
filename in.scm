#;'#('(4 5) (1 2))                          ; vector with quoted list and non-quoted list

#;'#("B" 3 'cake cookie donut pie)                     ; vector with quoted symbol and non-quoted symbol

#;'(1 2 3 4 4 5 6 7 8 9 10 11 12 13 (14 15 (16)) 17 18 19 20) ;115 constants

#;'('''a)                                   ; quote madness

#;'''''b                                    ; quote madness #2

#;'("A")                                    ; vector with string - should remain a capital A and not a lowercase a

#;'asfgdsfgsdfhgwghrwghwefh1835             ; just a symbol

#;"string"                                 ; simple string

#;1/3                                       ; fraction

#;(if #t 1 2)                               ; if with 'else' clause

#;(if #f 3)                                 ; if without else clause, test evaluates to #f, should return void

#;`(1 2 'k)                                 ; quasiquote - not yet implemented

#;`(1 ,2 'k)                                ; quasiquote with unquote - not yet implemented

#;`(1 ,@2 'k)                               ; quasiquote with unquote-splicing - not yet implemented

#;(begin 1 2 3)                             ; sequence

#;(or)                                      ; or without sub-expressions

#;(or #f #f 'a)                             ; or with sub-expressions

#;(1 2 3)                                   ; applying non-procedure - ERROR

#;((lambda (x y z) x) 1 5 '3)              ; creating a lambda simple closure and applying it, return 1 - checks that args were pushed on the stack in the correct order
#;((lambda (x y z) z) 1 5 '3)              ; creating a lambda simple closure and applying it, return 3 - checks that args were pushed on the stack in the correct order

#;((lambda (x y z) x y) 1 2)                ; anonymous closure application on less arguments than required

#;((lambda (x y z) x y) 1 2 3 4)            ; anonymous closure application on more arguments than required

#;((lambda (a b c) ((lambda (e f) ((lambda (x y z) x) e f a)) a b)) 1 2 3)  ; tail position application - check the the env expansion works properly, should return 1
#;((lambda (a b c) ((lambda (e f) ((lambda (x y z) y) e f a)) a b)) 1 2 3)  ; tail position application - check the the env expansion works properly, should return 2
#;((lambda (a b c) ((lambda (e f) ((lambda (x y z) z) e f a)) a b)) 1 2 3)  ; tail position application - check the the env expansion works properly, should return 1


#;(define foo (lambda (x) x))               ; the following 3 lines test for closure creation, binding the closure to its position in the free var table, using it in a later definition and then using that later definition
#;(define goo (foo 5))
#;(foo goo)

#;(foo goo)                                 ; test for access to undefined variable 'goo'
#;(define goo (foo 5))
#;(define foo (lambda (x) x))

#;((lambda s 1))                            ; variadic without arguments - check that Nil is pushed by hand to the stack

#;((lambda (a . d) d) 1 2 3 4 6 7 8 9 10)      ; packing the optional arguments to a list - should return (2 . (3 . (4 . (6 . (7 . (8 . (9 . (10 . ()))))))))

#;((lambda s 1) 2 3 4)

#;((lambda (x) (set! x 5)) 1)               ; set! expressions return void - should print a newline in the prompt

#;(define v (if #f 1))                      ; void values that result from set!, set-car!, set-cdr! and define should print nothin, but if the void is wrapped in a pair or vector, it should print #<void>. this test should print: (#<void> . (#<void> . (#<void> . ())))
#;((lambda s s) v v v)
(and
 (boolean? #t)
 (boolean? #f)
 (not (boolean? 1234))
 (not (boolean? 'a))
 (symbol? 'b)
 (procedure? procedure?)
 ;(eq? (car '(a b c)) 'a)
 (= (car (cons 1 2)) 1)
 (integer? 1234)
 (char? #\a)
 (null? '())
 (string? "abc")
 (symbol? 'lambda)
 (vector? '#(1 2 3))
 (not (vector? 1234))
 (not (string? '#(a b c)))
 (not (string? 1234))
 (= 3 (vector-length '#(a #t ())))
 (pair? '(a . b))
 (not (pair? '()))
 (zero? 0)
 (not (zero? 234))
 (= 97 (char->integer (string-ref "abc" 0)))
 (let ((n 10000))
   (= n (string-length (make-string n))))
 (let ((n 10000))
   (= n (vector-length (make-vector n))))
 (let ((v '#(a b c)))
   (eq? 'c (vector-ref v 2)))
 (= 65 (char->integer #\A))
 (let ((string (make-string 2)))
   (string-set! string 0 (integer->char 97))
   (string-set! string 1 (integer->char 98))
   (eq? 'ab (string->symbol string)))
 (= 3 (remainder 7 4))
 (= 6 (* 1 2 3))
 (= 1 (*))
 (= 234 (* 234))
 (= 6 (+ 1 2 3))
 (zero? (+))
 (= 234 (+ 234))
 (= 1 (- 6 3 2))
 (< 1 2 3 4 5)
 (> 5 4 3 2 1))
