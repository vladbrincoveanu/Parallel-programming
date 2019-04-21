thr1(Q) :-
thread_create(worker(Q),Id1, []),
thread_create(worker(Q),Id2, []),
thread_create(worker(Q),Id3, []),
thread_join(Id1, S),
thread_join(Id2, S),
thread_join(Id3, S).
worker(Q) :-
thread_self(Id),
afisare(Id,Q).
afisare(_,0) :- !.
afisare(Id,I) :-
format('Thread (~d): ~d\n',[Id,I]),
I1 is I-1,
afisare(Id,I1).