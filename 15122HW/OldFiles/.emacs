;; To configure the C0 emacs mode, put the following three lines
;; into your .emacs configuration file:

;; Setup for c0-mode
(setq c0-root "/afs/andrew/course/15/122/")
(load (concat c0-root "c0-mode/c0.el"))

;; Other configuration options that are useful for C0/122
(column-number-mode) ;; Shows the column number in the status bar.
(show-paren-mode) ;; Highlights the matching paren.
(add-hook 'before-save-hook 'delete-trailing-whitespace) ;; No trailing spaces
(setq require-final-newline t) ;; Good practice.
(setq-default indent-tabs-mode nil) ;; Don't indent with tabs.

;; If indent-tabs-mode is off, untabify before saving
;; This gets rid of tabs that another program put there.
(add-hook 'write-file-hooks (lambda () (if (not indent-tabs-mode) (untabify (point-min) (point-max))) nil ))
