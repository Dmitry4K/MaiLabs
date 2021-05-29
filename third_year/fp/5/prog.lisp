; defenition of polyonom
(defclass polynom ()
	((var-symbol :initarg :var :reader var)
	 (term-list :initarg :terms :reader terms)))
	 
(defgeneric zerop1 (arg)
	(:method ((n number))   ; (= n 0)
		(zerop n)))

(defgeneric minusp1 (arg)
	(:method ((n number))   ; (< n 0)
		(minusp n)))	 
	 
(defmethod print-object ((p polynom) stream)
  (format stream "[PL (~s) ~:{~:[~:[+~;-~]~d~[~2*~;~s~*~:;~s^~d~]~;~]~}]"
          (var p)
          (mapcar (lambda (term)
                    (list (zerop1 (coeff term))
                          (minusp1 (coeff term))
                          (if (minusp1 (coeff term))
                              (abs (coeff term))
                              (coeff term))
                          (order term)
                          (var p)
                          (order term)))
                  (terms p))))

(defun same-variable-p (v1 v2)
	(and (symbolp v1) (symbolp v2) (eq v1 v2)))

(defun sub-terms-lists (tl1 tl2)
	(let 
	((i 0) (j 0 ) (ntl (list)))
		(do () 
			((or (= i (length tl1) ) (= j (length tl2))))
			(let ((term1 (nth i tl1)) (term2 (nth j tl2)))
				(if (= (order term1) (order term2))
				(let ()
				(setf ntl (join-term-to-list ntl (sub-terms term1 term2)))
				(setf i (+ 1 i))
				(setf j (+ 1 j))
				)
				(if (> (order term1) (order term2))
				(let ()
				(setf ntl (join-term-to-list ntl term1))
				(setf i (+ 1 i))
				)
				(let ()
				(setf ntl (join-term-to-list ntl (sub-terms (make-term :order (order term2) :coeff 0) term2)))
				(setf j (+ 1 j))
				)))
			)
		)
		(do () ((= i (length tl1)))
			(let ()
				(setf ntl (join-term-to-list ntl (nth i tl1)))
				(setf i (+ 1 i))
			)
		)
		(do () ((= j (length tl2)))
			(let ((cur-term (nth j tl2)))
				(setf ntl (join-term-to-list ntl (sub-terms (make-term :order (order cur-term) :coeff 0) cur-term)))
				(setf j (+ 1 j))
			)
		)
		(if (null ntl) (list (make-term :coeff 0 :order 0)) ntl)
	)
)

(defun join-term-to-list (ntl new-term)
	(append ntl (if (null new-term) NIL (list new-term)))
)

(defun sub-terms (t1 t2)
	(let ((c1 (coeff t1))
		  (c2 (coeff t2))
		  (o1 (order t1))
		  (o2 (order t2)))
		(if (and (= o1 o2) (not (= (- c1 c2) 0)))
			(make-term :order o1 :coeff (- c1 c2))
			NIL
		)
	)
)
	
;-----------------------

;defenition of term
(defun make-term (&key order coeff)
	(list order coeff))
	
(defun order (term) (first term))
(defun coeff (term) (second term))
;------------------

;lab function
(defgeneric sub2 (arg1 arg2)
	(:documentation "Вычитает из arg1 arg2"))
	
(defmethod sub2 ((arg1 number) (arg2 number))
	(- arg1 arg2))

(defmethod sub2 ((p1 polynom) (p2 polynom))
	(if (same-variable-p (var p1) (var p2)) 
		(make-instance 'polynom
			:var (var p1)
			:terms (sub-terms-lists (terms p1) (terms p2))				
		) 
		(error "Polynoms have: different variables")
	)
)
