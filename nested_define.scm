;test 1
((lambda (a) (define a 5) a) 1)

;input : 1
;expected result : 5
;$
;test 2

((lambda (a) (define add5 (lambda (b) (+ b 5))) (add5 a)) 2)

;input : 2
;expected result : 7
;$
;test 3

((lambda (a) (define id (lambda (b) b)) (define min1 (lambda (b) (- b 1))) (min1 (id a))) 3)

;input : 3
;expected result : 2
;$
;test 4

((lambda (a b c) (define plus (lambda (a b) (+ a b))) (define minus (lambda (a b) (- a b))) (minus (plus a b) c)) 3 3 2)

;input : 3 3 2
;expected result : 4

;$
;test 7

((lambda (a) (begin (define a 4) (define id1 (lambda  (a b) a))) (* a a)) 3)

;input : 3
;expected result : 16

;$
;test 8

((lambda (a) (begin (define a 4) (define id1 (lambda  (a b) a))) (* (id1 a a) a)) 3)

;input : 3
;expected result : 16
;$
;test 9

((lambda (a b) (begin (define iszero (or (zero? a) (zero? b))) (define id1 (lambda (a b) a))) (if iszero 3 5)) 4 0)

;input : 4 0
;expected result : 3
;$
;test 10

((lambda (a b) (begin (define iszero (lambda (a b) (or (zero? a) (zero? b)))) (define min1 (lambda (a) (- a 1))) (define min2 (lambda (b) (- b 2)))) (if (iszero a b) (min1 a) (min2 b))) 3 0)

;input : 3 0
;expected result : 2
;$
;test 11

((lambda (a) (define min4 (lambda (a) (define min3 (lambda (a) (define min2 (lambda (a) (- a 2))) (-(min2 a) 1))) (- (min3 a) 1))) (+ (min4 a) a)) 5)

;input : 5
;expected result : 6
;$
;test 12

((lambda (t i)
      (begin
        (begin
          (define l (lambda (x) (+ x x)))
          (define o (lambda (y) (* y y))))
        (begin
          (define v (lambda (x) (+ x x)))
          (define e (lambda (y) (- y)))))
      (* i t)) 1 2)

;input : 1 2
;expected result : 2
;$
;test 13

((lambda (a)
	  (begin
		(begin
		  (define force (lambda (x) (+ x x)))
		  (define the  (lambda (y) (* y y)))
		  (define with (lambda (x) (- x 4))))
		(begin
		  (define may (lambda (x) (+ x x)))
		  (define be (lambda (y) (* 5 y))))
		(define you a))
	  (may (the (force (be (with you)))))) 10)

;input : 10
;expected result : 7200

;$

;test 20

((lambda (a b c)
  (begin
    (begin
      (begin
        (begin
          (begin
            (define x a))
          (define xx b)
          (define xxx (* b b))
         )))
    (* x xx xxx b a))) 4 4 4)

;input : 4 4 4
;expected result : 4096
;$
;test 21

((lambda (a b c)
  (begin
    (begin
      (begin
        (begin
          (begin
            (define x a))
          (define xx b)
          (define xxx (* b b))
         )))
    (begin
      (define xxxx c)
    (* x xx xxx xxxx b a)))) 4 4 4)

;input : 4 4 4
;expected result : 16384
;$
;test 22

((lambda (a b c)
  (begin
    (begin
      (begin
        (begin
          (begin
            (define x a))
          (define xx b)
          (define xxx (* b b))
         )))
    (begin
      (define xxxx c)
      (define tt (lambda (d) (begin
                            (define t 8) (* t 7))))
    (* x xx xxx (tt xxxx) b a)))) 4 4 4)

;input : 4 4 4
;expected result : 229376
;$
;test 23

((lambda (a b c)
  (begin
    (begin
      (define ff (lambda (g)
                   (begin
                     (define t (+ 6 g))
                     (define u (lambda (c) (* c (t c)))))
                   (- t g)))
      (begin
        (begin
          (begin
            (define x a))
          (define xx b)
          (define xxx (* b b))
         )))
    (begin
      (define xxxx c)
      (define tt (lambda (d) (begin
                            (define t 8) (* t 7))))

    (* x xx (ff c) (tt xxxx) b a)))) 4 4 4)

;input : 4 4 4
;expected result : 86016
;$
;test 24

((lambda (a b c)
  (begin
    (begin
      (define ff (lambda (g)
                   (begin
                     (define t (+ 6 g))
                     (define u (lambda (c) (* c (t c)))))
                   (- t g)))
      (begin
        (begin
          (begin
            (define x a))
          (define xx b)
          (define xxx (* b b))
          (define t1 (lambda (x)
                       (define t2 (lambda(x)
                                    (define t3 (lambda (x)
                                                 (define t4 (lambda (x)x))
                                                 (- 5 (t4 x))))
                                    (+ x (t3 7))))
                       (- (t2 a) a)))

         )))
    (begin
      (define xxxx c)
      (define tt (lambda (d) (begin
                            (define t 8) (* t 7))))

    (* x (t1 xx) (ff c) (tt xxxx) b a)))) 4 4 4)

;input : 4 4 4
;expected result : -43008
;$
;test 25

((lambda (a b c)
  (begin
    (begin
      (define ff (lambda (g)
                   (begin
                     (define t (+ 6 g))
                     (define u (lambda (c) (* c (t c)))))
                   (- t g)))
      5)

    (* a (ff b) b a))) 4 4 4)

;input : 4 4 4
;expected result : 384
;$
;test 26

((lambda (a b c)
  (begin
    (begin
      (define ff (lambda (g)
                   (begin
                     (define t (+ 6 g))
                     (define u (lambda (c) (* c (t c)))))
                   (- t g)))
      (ff 5)))
     (ff b)) 4 4 4)

;input : 4 4 4
;expected result : 6
;$
;test 27

((lambda (a b c)
  (begin
    (begin
      (define ff (lambda (g)
                   (begin
                     (define t (+ 6 g))
                     (define u (lambda (c) (* c (t c)))))
                   4))
      (ff 5)))
     (ff b)) 4 4 4)

;input : 4 4 4
;expected result : 4
;$
;test 28

((lambda (a b c)
  (begin
    (begin
      (define ff (lambda (g)
                   (begin
                     (define t (+ 6 g))
                     (define u (lambda (c) (* c t ))))
                   (u t)))
      (ff 5))
     (ff b))) 4 4 4)

;input : 4 4 4
;expected result : 100
;$
;test 29

((lambda (a) (begin) (define b (+ 87 a)) b) 4)

;input : 4
;expected result : 91
;$
;test 30

((lambda (a)(define b 6) b) 4)

;input : 4
;expected result : 6
