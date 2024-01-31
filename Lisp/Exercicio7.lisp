(defun concatenacao (L1 L2)

  (cond
      ( (null L1) L2)
      ( (null L2) L1)
 
  (t  (cons (first L1) (concatenacao (rest L1) L2) )   )

  )

)

(defun inverte (L)
  (if (null L)
      '()
    (concatenacao (inverte (rest L)) (list (first L)) )
  )
)

(defun ultimoElem (L)

   (cond
        ( (null L) '())
      (  (null (rest L))  (first L) )
      (t  (ultimoElem (rest L))   )
   )
)