(defun subsets (lst)
	(ss-ht (list) lst T)	;lst -> nil | lst
)
(defun ss-ht(head tail p)
	(cond
		;условие окончания рекурсии
		((null tail)
			(if p (list head))
		)
		(t (append 
			;head + first tail + rest tail = head + tail
			;передаем nil в третий аргумент, чтобы избежать повтора
			(ss-ht (append head (list (first tail))) (rest tail) nil)
			;head + rest tail != head + tail
			;новый список, выводим его
			(ss-ht head (rest tail) T)
			;секция с выводом списка
			(if p (list (append head tail)))
			)
		)
	)
)
