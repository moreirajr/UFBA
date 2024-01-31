;;exercicio 5******************************
(defun substituir (a c L)

 (cond
  ( (null L) '() )
  ( (equal a (first L) )   (cons c (substituir a c(rest L)) )         )
   
    (    t (cons (first L) (substituir a c (rest L))       )  )
   
   
   )

  )

;;ex 6**************************************

(defun remocao (A L)

   (cond
        ( (null L) '())    ;; lista vazia
        ( (eq A (first L))   (rest L) )   ;;remover o primeiro elemento
        (t  (cons (first L) (remocao A (rest L))  )    ) 
   )       ;; /\  concatena o primeiro (pra nao perder o valor)
)


;;exercicio 7 ***************************


(defun concatenacao (L1 L2)

  (cond
      ( (null L1) L2)
      ( (null L2) L1)
 
  (t  (cons (first L1) (concatenacao (rest L1) L2) )   )

  )
)

;;lista *******************

(defun inverte (L)
  (if (null L)
      '()
    (concatenacao (inverte (rest L)) (list (first L)) )
  )
)

;;**********************

(defun ultimoElem (L)

   (cond
        ( (null L) '())
      (  (null (rest L))  (first L) )
      (t  (ultimoElem (rest L))   )
   )
)


;;outros
;;questao 1  *************

(defun soma (L)

 (cond
   ( (null L) 0)
   ( t  (+ (first L) (soma (rest L)) )   )
 )
)

;;*****************************
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

;;**************************************
(defun foo (a b &optional c)
  (list a b c)
)

;;*********************************

(defun intercalar (L1 L2)

 (cond
     ( (null L1) L2)
     ( (null L2) L1)

     (t (cons (first L1) (cons (first L2) (intercalar (rest L1) (rest L2)) )   )  )
 )
)

;;*******************************

(defun addNoFinal (n L)

 (cond
     ( (null L) (list n))
     (t (cons (first L)(addNoFinal n (rest L) )  ) )
 )
)

;;********************************
(defun parearLista (x L)

  (cond
    ( (null L) '())
     (t (cons (first L) (cons x (parearLista x (rest L)))  )  )
  )
)

;;********************************

(defun parearList (x L)

  (cond
   ( (null L) '() )
   (t   (cons (first L) (cons x (parearList x (rest L))  )   )  )  
  )
)