(defun is-triangle(a b c)
  (and (> (+ a b) c) (> (+ b c) a)(> (+ c a) b))
)

(defun square(x) (* x x))

(defun angle-opposite-side(a b c)
  (acos (/ (- (+ (square b) (square c))(square a))(* 2 b c)))
)

(defun acute-angled(a b c)
  (and 
	(is-triangle a b c)
	(< (angle-opposite-side a b c) (/ pi 2))
	(< (angle-opposite-side b c a) (/ pi 2))
	(< (angle-opposite-side c a b) (/ pi 2))
  )
)