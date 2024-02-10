# TrainScheduler
Sa se implementeze un server ce ofera sau actualizeaza informatii in timp real de la toti clientii inregistrati pentru: mersul trenurilor, status plecari, status sosiri, intarzieri si estimare sosire. Serverul citeste datele din fisiere xml si actualizeaza datele(intarzieri si estimare sosire) la cererea clientilor (signal - receive) Toata logica va fi realizata in server, clientul doar cere informatii despre plecari/sosiri si trimite informatii la server despre posible intarzieri si estimare sosire.
Cuvinte cheie : Command design pattern, command queue, threads, sockets

    Activitati:
    trimitere de la server a informatiilor despre mersul trenurilor in ziua respectiva
    trimitere de la server a informatiilor despre plecari in urmatoarea ora (conform cu planul , in intarziere cu x min) doar la cererea unui client
    trimitere de la server a informatiilor despre sosiri in urmatoarea ora (conform cu planul , in intarziere cu x min, cu x min mai devreme) doar la cererea unui client

