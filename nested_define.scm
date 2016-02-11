
;;test 25
;
;((lambda (a b c)
;  (begin
;    (begin
;      (define ff (lambda (g)
;                   (begin
;                     (define t (+ 6 g))
;                     (define u (lambda (c) (* c (t c)))))
;                   (- t g)))
;      5)
;
;    (* a (ff b) b a))) 4 4 4)
;
;;input : 4 4 4
;;expected result : 384
;
;;test 26
;
;((lambda (a b c)
;  (begin
;    (begin
;      (define ff (lambda (g)
;                   (begin
;                     (define t (+ 6 g))
;                     (define u (lambda (c) (* c (t c)))))
;                   (- t g)))
;      (ff 5)))
;     (ff b)) 4 4 4)
;
;;input : 4 4 4
;;expected result : 6
;
;;test 27
;
;((lambda (a b c)
;  (begin
;    (begin
;      (define ff (lambda (g)
;                   (begin
;                     (define t (+ 6 g))
;                     (define u (lambda (c) (* c (t c)))))
;                   4))
;      (ff 5)))
;     (ff b)) 4 4 4)

;;input : 4 4 4
;;expected result : 4
;
;;test 28
;
;((lambda (a b c)
;  (begin
;    (begin
;      (define ff (lambda (g)
;                   (begin
;                     (define t (+ 6 g))
;                     (define u (lambda (c) (* c t ))))
;                   (u t)))
;      (ff 5))
;     (ff b))) 4 4 4)
;;
;;input : 4 4 4
;;expected result : 100
