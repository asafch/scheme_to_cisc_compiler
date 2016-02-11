
;test 6

(lambda ( x y z zx xx)
  (begin
  (define t1
    (lambda (g t)
               (if (zero? y) zx (g t z))))
    (define y (lambda (y x)
                (+ y x)))
    (define g (lambda (y f)
                (y f))))
    (set! y x) (t1 (y z g) g))

;BoxSet' (VarParam' ("y" : 2
;BoxGet' (VarBound' ("y" : 1
;Box' (VarParam' ("y" : 1
;BoxGet' (VarParam' ("y" : 1

output:
LambdaSimple' (["x"; "y"; "z"; "zx"; "xx"],                                                         Applic'
  (LambdaSimple' (["t1"; "y"; "g"],
    Seq'
     [Set' (Var' (VarParam' ("y", 1)), Box' (VarParam' ("y", 1)));
      Seq'
       [Set' (Var' (VarParam' ("t1", 0)),
         LambdaSimple' (["g"; "t"],
          If' (Applic' (Var' (VarFree' "zero?"), [BoxGet' (VarBound' ("y", 0, 1))]),
           Var' (VarBound' ("zx", 1, 3)),
           Applic' (Var' (VarParam' ("g", 0)),
            [Var' (VarParam' ("t", 1)); Var' (VarBound' ("z", 1, 2))]))));
        BoxSet' (VarParam' ("y", 1),
         LambdaSimple' (["y"; "x"],
          Applic' (Var' (VarFree' "+"), [BoxGet' (VarParam' ("y", 0)); Var' (VarParam' ("x", 1))])));
        Set' (Var' (VarParam' ("g", 2)),
         LambdaSimple' (["y"; "f"],
          Applic' (BoxGet' (VarParam' ("y", 0)), [Var' (VarParam' ("f", 1))])));
        BoxSet' (VarParam' ("y", 1), Var' (VarBound' ("x", 0, 0)));
        Applic' (Var' (VarParam' ("t1", 0)),
         [Applic' (BoxGet' (VarParam' ("y", 1)),
           [Var' (VarBound' ("z", 0, 2)); Var' (VarParam' ("g", 2))]);
          Var' (VarParam' ("g", 2))])]]),
  [Const' (Bool false); Const' (Bool false); Const' (Bool false)]))

;test 18

(lambda (a b c)
  (begin
    (begin
      (begin
        (define foo (lambda (x) (set! foo 4)(+ x x)))
        (define goo (lambda (y)
                      (define foo (* a b)) (* foo b)))))
    (define t5 (lambda (x) (begin
                             (define x 6)
							 (set! goo 5)
							 (set f 50)
                             (* a x))))
    (begin
      (define f (lambda (d)
                  (* d d d )))
      (begin
        (define h (lambda (f g)
                    (if (or (even? g) (even? g) (even (+ f g)))
                        f g)))))
    )
    (* (foo (goo (* a x)))
     (f (* b c))))


output:
LambdaSimple' (["a"; "b"; "c"],                                                                     Applic'
  (LambdaSimple' (["foo"; "goo"; "t5"; "f"; "h"],
    Seq'
     [Set' (Var' (VarParam' ("foo", 0)), Box' (VarParam' ("foo", 0)));
      Set' (Var' (VarParam' ("goo", 1)), Box' (VarParam' ("goo", 1)));
      Set' (Var' (VarParam' ("f", 3)), Box' (VarParam' ("f", 3)));
      Seq'
       [BoxSet' (VarParam' ("foo", 0),
         LambdaSimple' (["x"],
          Seq'
           [BoxSet' (VarBound' ("foo", 0, 0), Const' (Number (Int 4)));
            Applic' (Var' (VarFree' "+"), [Var' (VarParam' ("x", 0)); Var' (VarParam' ("x", 0))])]));
        BoxSet' (VarParam' ("goo", 1),
         LambdaSimple' (["y"],
          Applic'
           (LambdaSimple' (["foo"],
             Seq'
              [BoxSet' (VarParam' ("foo", 0),
                Applic' (Var' (VarFree' "*"),
                 [Var' (VarBound' ("a", 2, 0)); Var' (VarBound' ("b", 2, 1))]));
               Applic' (Var' (VarFree' "*"),
                [BoxGet' (VarParam' ("foo", 0)); Var' (VarBound' ("b", 2, 1))])]),
           [Const' (Bool false)])));
        Set' (Var' (VarParam' ("t5", 2)),
         LambdaSimple' (["x"],
          Applic'
           (LambdaSimple' (["x"],
             Seq'
              [Set' (Var' (VarParam' ("x", 0)), Const' (Number (Int 6)));
               BoxSet' (VarBound' ("goo", 1, 1), Const' (Number (Int 5)));
               Applic' (Var' (VarFree' "set"),
                [BoxGet' (VarBound' ("f", 1, 3)); Const' (Number (Int 50))]);
               Applic' (Var' (VarFree' "*"),
                [Var' (VarBound' ("a", 2, 0)); Var' (VarParam' ("x", 0))])]),
           [Const' (Bool false)])));
        BoxSet' (VarParam' ("f", 3),
         LambdaSimple' (["d"],
          Applic' (Var' (VarFree' "*"),
           [Var' (VarParam' ("d", 0)); Var' (VarParam' ("d", 0)); Var' (VarParam' ("d", 0))])));
        Set' (Var' (VarParam' ("h", 4)),
         LambdaSimple' (["f"; "g"],
          If'
           (Or'
             [Applic' (Var' (VarFree' "even?"), [Var' (VarParam' ("g", 1))]);
              Applic' (Var' (VarFree' "even?"), [Var' (VarParam' ("g", 1))]);
              Applic' (Var' (VarFree' "even"),
               [Applic' (Var' (VarFree' "+"),
                 [BoxGet' (VarParam' ("f", 0)); Var' (VarParam' ("g", 1))])])],
           BoxGet' (VarParam' ("f", 0)), Var' (VarParam' ("g", 1)))));
        Applic' (Var' (VarFree' "*"),
         [Applic' (BoxGet' (VarParam' ("foo", 0)),
           [Applic' (BoxGet' (VarParam' ("goo", 1)),
             [Applic' (Var' (VarFree' "*"), [Var' (VarBound' ("a", 0, 0)); Var' (VarFree' "x")])])]);
          Applic' (...); ...]);
        ...];
      ...]),
  ...))

;BoxSet' (VarBound' ("goo" : 1
;Box' (VarParam' ("f" : 1
;BoxSet' (VarBound' ("foo" : 1
;BoxSet' (VarParam' ("goo" : 1
;BoxSet' (VarParam' ("f" : 1
;BoxGet' (VarParam' ("goo" : 1
;BoxGet' (VarBound' ("f" : 1
;BoxGet' (VarParam' ("foo" : 1
;BoxSet' (VarParam' ("foo" : 1
;Box' (VarParam' ("foo" : 1
;$

;test 26

(define make-sq-jump (lambda (lst)
                      (letrec ((main-func (lambda (make-sq-jump res-lst)
											(lambda (a)
											 (set! make-sq-jump make-sq-jump)
											 (set! res-lst res-lst)
											 (if (null? (cdr (memq a lst)))
                                             (list lst)
                                             (let ((res-lst (make-sq-jump (cdr (memq a lst)))))
                                               (append (list (append (list a )
                                                                       (filter (lambda (res-lst-element)
                                                                                 ( > (car res-lst-element) (* a a)))
                                                                               res-lst )))
                                                       res-lst )))))))
                             (main-func  (car lst)))))

;BoxSet' (VarBound' ("res-lst" : 1
;BoxSet' (VarBound' ("make-sq-jump" : 1
;BoxGet' (VarBound' ("make-sq-jump" : 2
;Box' (VarParam' ("res-lst" : 1
;Box' (VarParam' ("make-sq-jump" : 1
;BoxGet' (VarBound' ("res-lst" : 1


output:
Def' (Var' (VarFree' "make-sq-jump"),                                                               LambdaSimple' (["lst"],
  Applic'
   (LambdaSimple' (["main-func"],
     Seq'
      [Set' (Var' (VarParam' ("main-func", 0)),
        LambdaSimple' (["make-sq-jump"; "res-lst"],
         Seq'
          [Set' (Var' (VarParam' ("make-sq-jump", 0)), Box' (VarParam' ("make-sq-jump", 0)));
           Set' (Var' (VarParam' ("res-lst", 1)), Box' (VarParam' ("res-lst", 1)));
           LambdaSimple' (["a"],
            Seq'
             [BoxSet' (VarBound' ("make-sq-jump", 0, 0),
               BoxGet' (VarBound' ("make-sq-jump", 0, 0)));
              BoxSet' (VarBound' ("res-lst", 0, 1), BoxGet' (VarBound' ("res-lst", 0, 1)));
              If'
               (Applic' (Var' (VarFree' "null?"),
                 [Applic' (Var' (VarFree' "cdr"),
                   [Applic' (Var' (VarFree' "memq"),
                     [Var' (VarParam' ("a", 0)); Var' (VarBound' ("lst", 2, 0))])])]),
               Applic' (Var' (VarFree' "list"), [Var' (VarBound' ("lst", 2, 0))]),
               Applic'
                (LambdaSimple' (["res-lst"],
                  Applic' (Var' (VarFree' "append"),
                   [Applic' (Var' (VarFree' "list"),
                     [Applic' (Var' (VarFree' "append"),
                       [Applic' (Var' (VarFree' "list"), [Var' (VarBound' ("a", 0, 0))]);
                        Applic' (Var' (VarFree' "filter"),
                         [LambdaSimple' (["res-lst-element"],
                           Applic' (Var' (VarFree' ">"),
                            [Applic' (Var' (VarFree' "car"),
                              [Var' (VarParam' ("res-lst-element", 0))]);
                             Applic' (Var' (VarFree' "*"),
                              [Var' (VarBound' ("a", 1, 0)); Var' (VarBound' ("a", 1, 0))])]));
                          BoxGet' (VarParam' ("res-lst", 0))])])]);
                    BoxGet' (VarParam' ("res-lst", 0))])),
                [Applic' (BoxGet' (VarBound' ("make-sq-jump", 0, 0)),
                  [Applic' (Var' (VarFree' "cdr"),
                    [Applic' (Var' (VarFree' "memq"),
                      [Var' (VarParam' ("a", 0)); Var' (VarBound' ("lst", 2, 0))])])])]))])]));
       Applic' (Var' (VarParam' ("main-func", 0)),
        [Applic' (Var' (VarFree' "car"), [Var' (VarBound' ("lst", 0, 0))])])]),
   [Const' (Bool false)])))
