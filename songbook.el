(defun music-mode-build-latex ()
  "Build LaTeX leadsheet file from music file"
  (interactive)
  (setq latex-buffer (generate-new-buffer "song.tex"))

  (setq lines
        (split-string
         (let* ((text (buffer-string))
                (start 0)
                (end (length text)))
           (set-text-properties start end nil text)
           text)
         "\n"))

  (while lines
    (with-current-buffer latex-buffer (insert (car lines)))
    (setq lines (cdr lines))
    )
  )











