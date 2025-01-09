(defmacro (defun name args expr) 
    (cons 'define 
        (cons name 
            (cons 
                (cons 'lambda 
                    (cons args 
                        (cons expr NIL)
                    )
                ) 
            NIL)
        )
    )
)

(defmacro (setq name val)
    (list 'define name val)
)

(define square (lambda (x) (* x x)))
(define abs (lambda (x) (- 0 x)))
(define foldl 
    (lambda (proc init list)
        (if list
            (foldl proc
                (proc init (car list))
                (cdr list)
            )
            init
        )
    )
)
(define foldr
    (lambda (proc init list)
        (if list
            (proc (car list)
                (foldr proc init (cdr list))
            )
            init
        )
    )
)

(define list (lambda items
    (foldr cons nil items))
)

(define reverse (lambda (list)
    (foldl (lambda (a x) (cons x a) nil list))
))

(defun unary-map (proc list)
    (foldr (lambda (x rest) (cons (proc x) rest))
        nil
        list
    )
)

(defun map (proc . arg-lists)
    (if (car arg-lists)
        (cons (apply proc (unary-map car arg-lists))
            (apply map (cons proc
                (unary-map cdr arg-lists))))
        nil
    )
)

(defun append (a b) (foldr cons b a))
(defun caar (x) (car (car x)))
(defun cadr (x) (car (cdr x)))

(defmacro (quasiquote x)
    (if (pair? x)
        (if (eq? (car x) 'unquote)
            (cadr x)
            (if (eq? (caar x) 'unquote-splicing)
                (list 'append
                    (cadr (car x))
                    (list 'quasiquote (cdr x)))
                (list 'cons
                    (list 'quasiquote (car x))
                    (list 'quasiquote (cdr x)))))
        (list 'quote x)))

(defmacro (let defs . body) 
    `((lambda ,(map car defs) ,@body)
        ,@(map cadr defs)))

(defun fib (n a b) 
    (if (= n 0) 
        nil 
        (cons a (fib (- n 1) b (+ a b)))))
