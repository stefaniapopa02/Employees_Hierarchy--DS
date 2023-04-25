#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_no_subordinates 100

#include "hierarchy.h"

Tree find_employee(Tree tree, char *name)
{
    if (tree == NULL)
    {
        return NULL;
    }
    if (strcmp(tree->name, name) == 0)
    {
        return tree;
    }

    int i, nr = tree->direct_employees_no;
    if (nr == 0)
    {
        return NULL;
    }
    for (i = 0; i < nr; i++)
    {
        Tree res = find_employee(tree->team[i], name);
        if (res != NULL)
        {
            return res;
        }
    }

    return NULL;
}

Tree *alphabetic(Tree *team, int employees_no)
{
    if (team == NULL)
    {
        return NULL;
    }
    int i, j;
    Tree aux;
    for (i = 0; i < (employees_no - 1); i++)
    {
        for (j = i + 1; j < employees_no; j++)
        {
            if (strcmp(team[i]->name, team[j]->name) > 0)
            {
                aux = team[i];
                team[i] = team[j];
                team[j] = aux;
            }
        }
    }
    return team;
}

/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */
Tree hire(Tree tree, char *employee_name, char *manager_name)
{

    Tree new_employee = (Tree)malloc(sizeof(TreeNode));
    new_employee->name = (char *)malloc(450 * sizeof(char));
    strcpy(new_employee->name, employee_name);
    new_employee->direct_employees_no = 0;
    new_employee->team = NULL;
    new_employee->manager = NULL;

    if (tree == NULL && manager_name == NULL)
    {
        tree = new_employee;
        return tree;
    }

    Tree manager = find_employee(tree, manager_name);

    if (manager == NULL)
    {
        return tree;
    }
    else
    {
        new_employee->manager = manager;
        int nr = manager->direct_employees_no;
        if (nr == 0)
        {
            manager->team = malloc(sizeof(Tree));
        }
        else
        {
            manager->team = realloc(manager->team, (nr + 1) * sizeof(Tree));
        }
        manager->team[nr] = new_employee;
        manager->direct_employees_no++;
        manager->team = alphabetic(manager->team, manager->direct_employees_no);
    }

    return tree;
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire(Tree tree, char *employee_name)
{

    Tree employee = find_employee(tree, employee_name);
    if (employee == NULL || employee->manager == NULL)
    {
        return tree;
    }
    int i, poz = 0, nr_m = employee->manager->direct_employees_no;
    Tree manager = employee->manager;
    for (i = 0; i < nr_m; i++)
    {
        if (strcmp(manager->team[i]->name, employee_name) == 0)
        {
            poz = i;
            break;
        }
    }
    for (i = (poz + 1); i < nr_m; i++)
    {
        manager->team[i - 1] = manager->team[i];
    }
    manager->direct_employees_no--;
    nr_m = manager->direct_employees_no;
    if (employee->direct_employees_no == 0)
    {
        return tree;
    }
    else
    {

        int nr_e = employee->direct_employees_no;
        manager->team = realloc(manager->team, (nr_m + nr_e) * sizeof(Tree));
        for (i = 0; i < nr_e; i++)
        {
            manager->team[nr_m] = employee->team[i];
            employee->team[i]->manager = manager;
            nr_m++;
        }
        manager->direct_employees_no = nr_m;
        manager->team = alphabetic(manager->team, nr_m);

        return tree;
    }
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name)
{

    Tree employee = find_employee(tree, employee_name);

    if (employee == NULL || employee->manager == NULL || employee->manager->manager == NULL)
    {
        return tree;
    }
    Tree manager = employee->manager;

    int nr = manager->manager->direct_employees_no;
    manager->manager->team = realloc(manager->manager->team, (nr + 1) * sizeof(Tree));
    manager->manager->team[nr] = employee;

    manager = fire(manager, employee_name);
    employee->team = NULL;
    employee->direct_employees_no = 0;

   employee->manager = manager->manager;


    manager->manager->direct_employees_no++;
    nr = manager->manager->direct_employees_no;
    manager->manager->team = alphabetic(manager->manager->team, nr);

    return tree;
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name)
{
    Tree moved = find_employee(tree, employee_name);
    if( moved == NULL || moved->manager == NULL){
        return tree;
    }

    Tree new_manager = find_employee(tree, new_manager_name);
    if(new_manager == NULL){
        return tree;
    }

    Tree manager = moved->manager;

    if(moved->manager == new_manager){
        return tree;
    }

    int nr = new_manager->direct_employees_no;
    new_manager->team = realloc(new_manager->team, (nr + 1) * sizeof(Tree));
    new_manager->team[nr] = moved;

    manager = fire(manager, employee_name);
    moved->team = NULL;
    moved->direct_employees_no = 0;
    moved->manager = new_manager;

    new_manager->direct_employees_no++;
    nr = new_manager->direct_employees_no;
    new_manager->team = alphabetic(new_manager->team, nr);

    return tree;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name)
{
    Tree employee = find_employee(tree, employee_name);

    if(employee == NULL || employee->manager == NULL){
        return tree;
    }

    Tree new_manager = find_employee(tree, new_manager_name);

    if(new_manager == NULL || employee->manager == new_manager){
        return tree;
    }

    Tree manager = employee->manager;

    int nr = new_manager->direct_employees_no;
    new_manager->team = realloc(new_manager->team, (nr + 1) * sizeof(Tree));
    new_manager->team[nr] = employee;

    new_manager->direct_employees_no++;
    nr = new_manager->direct_employees_no;
    employee->manager = new_manager;

    new_manager->team = alphabetic(new_manager->team, nr);

    nr = manager->direct_employees_no;
    int i, poz = 0;
    for( i = 0; i < nr; i++){
        if(strcmp(manager->team[i]->name, employee_name) == 0){
            poz = i;
            break;
        }
    }
    for(i = poz + 1; i < nr; i++){
        manager->team[i - 1] = manager->team[i];
    }

    manager->direct_employees_no--;

    return tree;
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire_team(Tree tree, char *employee_name)
{
    Tree fired = find_employee(tree, employee_name);

    if(fired == NULL || fired->manager == NULL){
        return tree;
    }

    Tree manager = fired->manager;

    int nr = manager->direct_employees_no;
    int i, poz = 0;
    for( i = 0; i < nr; i++){
        if(strcmp(manager->team[i]->name, employee_name) == 0){
            poz = i;
            break;
        }
    }
    for(i = poz + 1; i < nr; i++){
        manager->team[i - 1] = manager->team[i];
    }

    manager->direct_employees_no--;


    return tree;
}


void add_employees_by_manager(Tree tree, Tree *team, int *no){
       
        team[*no] = tree;
        (*no)++;
        int d = tree->direct_employees_no;
        if(d == 0){
            return ;
        }
        int i;
        for(i = 0; i < d; i++){
             add_employees_by_manager(tree->team[i], team, no);
        }
        return ;
}


/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
void get_employees_by_manager(FILE *f, Tree tree, char *employee_name)
{
    Tree employee = find_employee(tree, employee_name);

    if(employee == NULL ){
        fprintf(f, "\n");
        return;
    }

   Tree *team = (Tree*)malloc(100 * sizeof(Tree));
    int *no = (int*)malloc(sizeof(int));
    *no = 0;

    add_employees_by_manager(employee, team, no);
    team = alphabetic(team, *no);

    for(int i = 0; i < *no; i++){
        fprintf(f, "%s ", team[i]->name);
    }

    fprintf(f, "\n");

    return;
}


void add_employees_by_level(Tree tree, Tree *team, int *no, int level, int *crt_level){

    int i, d = tree->direct_employees_no;

    if((*crt_level + 1) == level){
        for( i = 0; i < d; i++){
            team[*no] = tree->team[i];
            (*no)++;
        }
        return;
    }
    if((*crt_level + 1) < level){
        if(d > 0){
            (*crt_level)++;
            for(i = 0; i < d; i++){
                add_employees_by_level(tree->team[i], team, no, level, crt_level);
                
            }
            (*crt_level)--;

        }
        
    }
    return;
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */
void get_employees_by_level(FILE *f, Tree tree, int level)
{
    if(level < 0){
        fprintf(f, "\n");
        return;
    }
    if(level == 0){
        fprintf(f, "%s \n", tree->name);
        return;
    }

    Tree *team = (Tree*)malloc(100 * sizeof(Tree));
    int *no = (int*)malloc(sizeof(int));
    *no = 0;
    int *crt_level = (int*)malloc(sizeof(int));
    *crt_level = 0;

    add_employees_by_level(tree, team, no, level, crt_level);

    if(*no == 0){
        fprintf(f, "\n");
        return;
    }
    else
    {
        team = alphabetic(team, *no);

        for(int i = 0; i < *no; i++){
            fprintf(f, "%s ", team[i]->name);
        }

        fprintf(f, "\n");

        return;
    }
    
}

void max_no_employees(Tree tree, int *max){

    int d = tree->direct_employees_no;

    if(d > *max){
        *max = d;
    }
    for(int i = 0; i < d; i++){
        max_no_employees(tree->team[i], max);
    }

    return;

}

void add_best_manager(Tree tree, Tree *team, int *no, int max){
    
    int d = tree->direct_employees_no;

    if(d == max){
        team[*no] = tree;
        (*no)++;
    }
    for(int i = 0; i < d; i++){
        add_best_manager(tree->team[i], team, no, max);
    }
    return;

}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void get_best_manager(FILE *f, Tree tree)
{
    if(tree == NULL){
        return;
    }
    int *max = (int *)malloc(sizeof(int));
    *max = 0;

    max_no_employees(tree, max);

    Tree* team = (Tree*)malloc(100 * sizeof(Tree));
    int *no = (int *)malloc(sizeof(int));
    *no = 0;

    add_best_manager(tree, team, no, *max);

    team = alphabetic(team, *no);

    for(int i = 0; i < *no; i++){
        fprintf(f, "%s ", team[i]->name);
    }

    fprintf(f, "\n");

    return;
}



Tree move(Tree new_tree, Tree employee, Tree new_manager){
    Tree manager = employee->manager;

    int nr = new_manager->direct_employees_no;
    new_manager->team = realloc(new_manager->team, (nr + 1) * sizeof(Tree));
    new_manager->team[nr] = employee;

    new_manager->direct_employees_no++;
    nr = new_manager->direct_employees_no;
    employee->manager = new_manager;

    new_manager->team = alphabetic(new_manager->team, nr);

    nr = manager->direct_employees_no;
    int i, poz = 0;
    for( i = 0; i < nr; i++){
        if(strcmp(manager->team[i]->name, employee->name) == 0){
            poz = i;
            break;
        }
    }
    for(i = poz + 1; i < nr; i++){
        manager->team[i - 1] = manager->team[i];
    }

    manager->direct_employees_no--;

    return new_tree;

}


void additional(Tree tree, Tree *new_tree, Tree *employee, Tree *manager){

    if(*new_tree == NULL){

        *new_tree = *employee;
        *manager = (*employee)->manager;
        (*new_tree)->manager = NULL;

        fire_team(tree, (*employee)->name);

        additional(tree, new_tree, manager, employee);
        return;
    }
    while(*employee != NULL){
        if((*employee)->manager == NULL){
            int nr = (*manager)->direct_employees_no;
            (*manager)->team = realloc((*manager)->team, (nr + 1) * sizeof(Tree));
            (*manager)->team[nr] = (*employee);

            (*manager)->direct_employees_no++;
            nr = (*manager)->direct_employees_no;
            (*employee)->manager = *manager;

            (*manager)->team = alphabetic((*manager)->team, nr);

            return;
        }
        fire_team(tree, (*manager)->name);
        Tree m_manager = (*manager)->manager;
       *new_tree = move(*new_tree, *manager, *employee );
        additional(tree, new_tree, &m_manager, manager);
    }
    return;

}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name)
{
    Tree employee = find_employee(tree, employee_name);

    if(employee == NULL || employee->manager == NULL){
        return tree;
    }
    
    Tree new_tree = NULL;
    Tree manager = NULL;
    
    additional(tree, &new_tree, &employee, &manager);

    return new_tree;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void preorder_traversal(FILE *f, Tree tree)
{
    if (tree == NULL)
    {
        return;
    }

    if (tree->manager == NULL)
    {
        fprintf(f, "%s ", tree->name);
    }
    else
    {
        fprintf(f, "%s-%s ", tree->name, tree->manager->name);
    }

    int nr = tree->direct_employees_no;
    if (nr == 0)
    {
        if (tree->manager == NULL)
            fprintf(f, "\n");
        return;
    }
    else
    {
        int i;
        for (i = 0; i < nr; i++)
        {
            preorder_traversal(f, tree->team[i]);
        }

        if (tree->manager == NULL)
            fprintf(f, "\n");
        return;
    }
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree)
{
    return;
}
