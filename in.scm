#;'#('(4 5) (1 2))                          ; vector with quoted list and non-quoted list

'#("B" 3 'cake cookie donut pie)                     ; vector with quoted symbol and non-quoted symbol

#; '(1 2 3 4 4 5 6 7 8 9 10 11 12 13 (14 15 (16)) 17 18 19 20) ;115 constants

#;'('''a)                                   ; quote madness

#;'''''b                                    ; quote madness #2

#;'("A")                                    ; vector with string - should remain a capital A and not a lowercase a

#;'asfgdsfgsdfhgwghrwghwefh1835             ; just a symbol

#;"string"                                 ; simple string

#;1/3                                       ; fraction

#;(if #t 1 2)                               ; if with 'else' clause

#;(if #f 3)                                 ; if without else clause, test evaluates to #f, should return #<void> as per write_sob_void.asm

#;`(1 2 'k)                                 ; quasiquote

#;`(1 ,2 'k)                                ; quasiquote with unquote

#;`(1 ,@2 'k)                               ; quasiquote with unquote-splicing

#;(begin 1 2 3)                             ; sequence

#;(or)                                      ; or without sub-expressions

#;(or #f #f 'a)                             ; or with sub-expressions

#;(1 2 3)                                   ; applying non-procedure - ERROR

#;((lambda (x y z) x) 1 5 '3)              ; creating a lambda simple closure and applying it, return 1 - checks that args were pushed on the stack in the correct order
#;((lambda (x y z) z) 1 5 '3)              ; creating a lambda simple closure and applying it, return 3 - checks that args were pushed on the stack in the correct order

#;((lambda (x y z) x y) 1 2)                ; anonymous closure application on less arguments than required

#;((lambda (x y z) x y) 1 2 3 4)            ; anonymous closure application on more arguments than required

#;((lambda (x) ((lambda (y) ((lambda (z) z) y)) x)) 2)         ; tail position application - check the the env expansion works properly

#;(define foo (lambda (x) x))               ; the following 3 lines test for closure creation, binding the closure to its position in the free var table, using it in a later definition and then using that later definition
#;(define goo (foo 5))
#;(foo goo)
