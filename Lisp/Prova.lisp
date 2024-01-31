;;questao 1

(defun soma (L)

 (cond
   ( (null L) 0)
   ( t  (+ (first L) (soma (rest L)) )   )
 )
)

;;questao 2
;; (reduzL '(1 2 ...) :i +)  ou (reduzL '(1 2...) :o 2),  ':i ou :o' = prox valor , (reduzL '(1 2) :o 3), o = 3
(defun reduzL (L &key i o)
  (cond
     ( (null L) (if (or (eq o +) (eq o -)) 0  1) )
     ( (null (rest L)) (first L) )
    (   (eq o +)  (+ (first L) (first (rest L))   (reduzL (rest(rest L)) :o +)  )  )
    (   (eq o -)  (- (first L) (first (rest L))   (reduzL (rest(rest L)) :o -)  )  )
    (   (eq o *)  (* (first L) (first (rest L))   (reduzL (rest(rest L)) :o *)  )  )
  )
)

(defun foo (a b &optional c)
  (list a b c)
)

(defun concatenar (L1 L2)

 (cond
   ((null L1) L2)
   ((null L2) L1)
  
 (cons (first L1) (concatenar (rest L1) L2))
 )
)