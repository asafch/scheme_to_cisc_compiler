#;'#('(4 5) (1 2))                          ; vector with quoted list and non-quoted list

#;'("B" 3 'cake cookie)                     ; vector with quoted symbol and non-quoted symbol

#;'("A")                                    ; vector with string - should remain a capital A and not a lowercase a

#;'asfgdsfgsdfhgwghrwghwefh1835             ; just a symbol


#;(if #t 1 2)                               ; if with 'else' clause

#;(if #f 3)                                 ; if without else clause, test evaluates to #f, should return #<void> as per write_sob_void.asm

#;`(1 2 'k)                                 ; quasiquote

#;`(1 ,2 'k)                                ; quasiquote with unquote

#;`(1 ,@2 'k)                               ; quasiquote with unquote-splicing

#;(begin 1 2 3)                             ; sequence

#;(or)                                      ; or without sub-expressions

#;(or #f #f 'a)                             ; or with sub-expressions

(1 2 3)                                     ; applying non-procedure - ERROR
