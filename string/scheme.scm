(struct Hoge
        (hoge (int unsigned))
        (emacs (char *emacs))
        (next (*Hoge)))

($fun (string_length (register char *s) inline extern)
      (register char *t)
      (= t s)
      (for ()
           ($do-write 4 (if (!*t) (return (- t s))) (++t) (++t))))

(loop:
 (printf "hoge%s" int hoge)
 (goto loop:))
