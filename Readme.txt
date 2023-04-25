Pe tot parcursul programului am folosit 2 functii aditionale importante:
---> find_employee = cauta intr-o ierarhie un angajat cu un anumit nume; daca nu il gaseste, returneaza NULL
                   = parcurge ierarhia prin recursivitate
---> alphabetic = functie ce sorteaza angajatii dintr-o echipa in ordine alfabetica


hire = aloc spatiu pentru noul angajat si campurile continand datele sale
     = daca este primul din ierarhie, ierarhia il va contine doar pe el 
     = daca nu, caut in ierarhie manager-ul corespunzator, si il adaug in echipa sa,
      marind spatiul echipei cu un loc si rearanjand angajatii in ordine alfabetica

fire = cautam in ierarhie angajatul pe care urmeaza sa-l concediem
     = daca nu il gasim, sau daca e primul din ierarhie, ierarhia ramane nemodificata
     = cautam pozitia pe care se afla angajatul in echipa din care face parte, si o reorganizam,
       scotandu-l din echipa
     = adaugam toti angajatii sai in echipa fostului sau manager, iar apoi o reordonam alfabetic

promote = cautam omul ce trebuie promovat in ierarhie
        = il adaugam in echipa managerului managerului sau, si il excludem din echipa fostului manager
        = toti angajatii sai vor trece in echipa fostului manager, in mod similar procedurii din functia fire

move_employee = cautam omul ce trebuie mutat
              = il adaugam in echipa noului manager, si il excludem din echipa fostului manager
              = toti angajatii sai vor trece in echipa fostului manager, in mod similar procedurii din functia fire

move_team = cautam reprezentantul echipei ce trebuie mutata
          = il adaugam in echipa noului manager, cu tot cu echipa sa, si il excludem din echipa fostului manager

fire_team = cautam in ierarhie angajatul pe care urmeaza sa-l concediem, impreuna cu echipa subordonata
          = daca nu il gasim, sau daca e primul din ierarhie, ierarhia ramane nemodificata
          = cautam pozitia pe care se afla angajatul in echipa din care face parte, si o reorganizam,
            scotandu-l din echipa, implicit cu tot cu angajatii sai

get_employees_by_manager = formam o echipa din toti subordonatii directi si indirecti ai angajatului corespunzator
                         = pentru parcurgerea tuturor subordonatilor si formarea echipei, cream functia 
                           add_employees_by_manager
                         = ordonam alfabetic echipa formata si o afisam

get_employees_by_level = formam o echipa din toti angajatii ierarhiei de pe nivelul cautat
                       = daca nivelul cautat nu exista in ierarhie, nu facem nimic 
                       = pentru selectarea tuturor angajatilor de pe un anumit nivel cream functia add_employees_by_level
                       = ordonam alfabetic echipa nou creata si o afisam

get_best_manager = pentru aflarea numarului maxim de subordonati directi ai unui angajat, cream functia max_no_employees
                 =  selectam toto angajatilor cu numarul maxim de subordonati si ii adaugam intr-o echipa nou creata cu 
                    ajutorul functiei add_best_manager
                 = sortam alfabetic echipa nou creata si o afisam


        