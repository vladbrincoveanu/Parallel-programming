wilsonPrimes :-
  threadLoopSimple(1,9,35000).

threadLoopSimple(N,N,Length):-!.
threadLoopSimple(A,N,Length):-
  format('\n(~w) START--------\n',A),
  threadCreateLoop(0,A,Length,R),
  threadJoinLoop(R),
  A1 is A + 1,
  format('\n(~w) END --------\n',A),
  threadLoopSimple(A1,N,Length).

threadCreateLoop(N,N,Length,[]):-!.
threadCreateLoop(A,N,Length,[Id|R]):-
  A < N,
  Start is A + 1,
  thread_create(worker([Start,Length,N,Rez]),Id,[]),
  A1 is A + 1,
  threadCreateLoop(A1,N,Length,R).

threadJoinLoop([]).
threadJoinLoop([H|T]):-
  thread_join(H,S),
  threadJoinLoop(T).

worker([StartInterval,EndInterval,Increment,Rez]) :-
  thread_self(Id),
  statistics(runtime, [H1|_]),
  loop(Id,StartInterval,EndInterval,Increment,R),
  append(R,[],Rez),
  statistics(runtime, [H2|_]),
  Time is H2 - H1,
  format('Thread (~w) TIME: (~w)\n',[Id,Time]).

loop(Id,StartInterval,EndInterval,Increment,[]):-
  StartInterval >= EndInterval,!.

loop(Id,StartInterval,EndInterval,Increment,[StartInterval|Rez]):-
  StartInterval < EndInterval,
  isPrime(StartInterval),
  checkWilson(StartInterval),
  afisare(Id,StartInterval),
  N is StartInterval + Increment,
  loop(Id,N,EndInterval,Increment,Rez).

loop(Id,StartInterval,EndInterval,Increment,Rez):-
  StartInterval < EndInterval,
  N is StartInterval + Increment,
  loop(Id,N,EndInterval,Increment,Rez).

isPrime(2):-!.
isPrime(3):-!.
isPrime(A):-
  A > 3,
  A mod 2 =\= 0,
  N is ceiling(sqrt(A)),
  isPrime(A,3,N).

isPrime(A,N,Max):-
  N >= Max -> true ;
  0 is A mod N -> false ;
  N1 is N + 2,
  isPrime(A,N1,Max).

checkWilson(A):-
  A1 is A - 1,
  factorial(A1,F),
  F1 is F + 1,
  A2 is A * A,
  0 is F1 mod A2. 

factorial(N, R) :- 
  factorial(N, 1, R).
factorial(0, R, R) :- !.
factorial(N, Acc, R) :-
  N1 is N - 1,
  Acc1 is Acc * N,
  factorial(N1, Acc1, R).

afisare(Id,I) :- format('Thread (~w): ~w\n',[Id,I]).
