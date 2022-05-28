# cminusParser

## 文法要求

```
1. program -> declaration_list
2. declaration_list -> declaration_list declaration  |  declaration
3. declaration -> var_declaration  |  fun_declaration
4. var_declaration -> type_specifier ID;  |  type_specifier ID [ NUM ]; 
5. type_specifier ->  int  |  void
6. fun_declaration -> type_specifier ID ( params )  compound_stmt

program ->  declaration  { declaration }
declaration ->  type_specifier  ID  declaration’
declaration’ ->  ;  |  [ NUM ];  |  ( params )  compound_stmt
type_specifier ->  int  |  void

7. params -> param_list  |  void
8. param_list -> param_list , param  |  param
9. param -> type_specifier ID  |  type_specifier ID [ ]

param_list -> param_list1  |  param_list2
param_list1->  void  [ ID  [ [  ] ]  {  , param  } ]
param_list2->  int  ID  [ [  ] ]  {  , param  }
param ->  type_specifier ID  [ [  ] ]

10. compound_stmt -> { local_declarations statement_list }
11. local_declarations -> local_declarations var_declaration | empty
12. statement_list -> statement_list statement  |  empty

compound_stmt -> {  { var_declaration }  { statement }  }

13. statement -> expression_stmt  |  compound_stmt  |  selection_stmt  |  iteration_stmt  |  return_stmt
14. expression_stmt -> expression ;  |  ;




15. selection_stmt -> if ( expression ) statement  |  if ( expression ) statement else statement

selection_stmt -> if ( expression ) statement  [ else statement ]

16. iteration_stmt -> while ( expression ) statement

17. return_stmt -> return  |  return  expression 
return_stmt ->  return  [  expression  ]

18. expression -> var = expression  |  simple_expression
19. var -> ID  |  ID [ expression ]
20. simple_expression -> additive_expression relop additive_expression  |  additive_expression
21. relop ->  <=  |  <  |  >  |  >=  |  ==  |  ~=
22. additive_expression -> additive_expression addop term  |  term
23. addop -> +  |  -
24. term -> term mulop factor  |  factor
25. mulop -> *  |  /
26. factor -> ( expression )  |  var  |  call  |  NUM
27. call -> ID ( args )

simple_expression可能以ID开头也可能以数组开头也可能不，会造成比较麻烦的左因子。改文法可以，程序处理也可以。
expression -> var = expression  |  simple_expression
var -> ID [  [expression]  ]
simple_expression -> additive_expression [ relop additive_expression ]
relop ->  <=  |  <  |  >  |  >=  |  ==  |  ~=
additive_expression ->  term  { addop term }
addop -> +  |  -
term ->  factor  { mulop factor }
mulop -> *  |  /
factor ->  ( expression )  |  ID  factor’  |  NUM
factor’ ->  [expression]  |  ( args )  |  ε


28. args -> arg_list  |  empty
29. arg_list -> arg_list , expression  |  expression
arg_list ->  expression   {  , expression }  |  ε
```

## test

input:

```
/* A program to perform Euclid's
Algorithm to compute gcd. */

int gcd (int u, int v)
{
	if (v == 0)
		return u ;
	else
		return gcd(v,u-u/v*v);
	/* u-u/v*v == u mod v */
}

void main(void)
{
	int x; int y;
	x = input();
	y = input();
	output(gcd(x,y));
}
```

output:

```
Syntax tree:
  FuncK
    IntK
    IdK: gcd
    ParamsK
      ParamK
        IntK
        IdK: u
      ParamK
        IntK
        IdK: v
    CompK
      If
        Op: ==
          IdK: v
          ConstK: 0
        Return
          IdK: u
        Return
          CallK
            IdK: gcd
            ArgsK
              IdK: v
              Op: -
                IdK: u
                Op: *
                  Op: /
                    IdK: u
                    IdK: v
                  IdK: v
  FuncK
    VoidK
    IdK: main
    ParamsK
      VoidK
    CompK
      Var_DeclK
        IntK
        IdK: x
      Var_DeclK
        IntK
        IdK: y
      Assign
        IdK: x
        CallK
          IdK: input
      Assign
        IdK: y
        CallK
          IdK: input
      CallK
        IdK: output
        ArgsK
          CallK
            IdK: gcd
            ArgsK
              IdK: x
              IdK: y
```

input:

```
/* A program to perform selection sort on a 10
 	element array. */
int x[10];
int minloc( int a[], int low, int high )
{ int i; int x; int k;
  k = low;
  x = a[low];
  i = low + 1;
  while(i < high)
    { if(a[i] < x)
        {  x = a[i];
          k = i; }
      i = i + 1;
    }
  return k;
}

void sort ( int a[], int low, int high )
{ int i; int k;
  int low;
  while (i < high-1)
    { int t;
      k = minloc (a,i,high);
      t =a[k];
      a[k] = a[i];
      a[i] = t;
      i = i + 1;
    }
}

void main(void)
{ int i;
  i = 0;
  while (i < 10)
    { x[i] = input;
      i = i + 1;
      sort (x,0,10);
      i = 0;
      while (i < 10)
        { output(x[i]);
          i = i + 1;
        }
    }
}
```

output:

```
CMINUS PARSING:

Syntax tree:
  Var_DeclK
    IntK
    Arry_DeclK
      IdK: x
      ConstK: 10
  FuncK
    IntK
    IdK: minloc
    ParamsK
      ParamK
        IntK
        IdK: a
        IdK:
      ParamK
        IntK
        IdK: low
      ParamK
        IntK
        IdK: high
    CompK
      Var_DeclK
        IntK
        IdK: i
      Var_DeclK
        IntK
        IdK: x
      Var_DeclK
        IntK
        IdK: k
      Assign
        IdK: k
        IdK: low
      Assign
        IdK: x
        Arry_ElemK
          IdK: a
          IdK: low
      Assign
        IdK: i
        Op: +
          IdK: low
          ConstK: 1
      While
        Op: <
          IdK: i
          IdK: high
        CompK
          If
            Op: <
              Arry_ElemK
                IdK: a
                IdK: i
              IdK: x
            CompK
              Assign
                IdK: x
                Arry_ElemK
                  IdK: a
                  IdK: i
              Assign
                IdK: k
                IdK: i
          Assign
            IdK: i
            Op: +
              IdK: i
              ConstK: 1
      Return
        IdK: k
  FuncK
    VoidK
    IdK: sort
    ParamsK
      ParamK
        IntK
        IdK: a
        IdK:
      ParamK
        IntK
        IdK: low
      ParamK
        IntK
        IdK: high
    CompK
      Var_DeclK
        IntK
        IdK: i
      Var_DeclK
        IntK
        IdK: k
      Var_DeclK
        IntK
        IdK: low
      While
        Op: <
          IdK: i
          Op: -
            IdK: high
            ConstK: 1
        CompK
          Var_DeclK
            IntK
            IdK: t
          Assign
            IdK: k
            CallK
              IdK: minloc
              ArgsK
                IdK: a
                IdK: i
                IdK: high
          Assign
            IdK: t
            Arry_ElemK
              IdK: a
              IdK: k
          Assign
            Arry_ElemK
              IdK: a
              IdK: k
            Arry_ElemK
              IdK: a
              IdK: i
          Assign
            Arry_ElemK
              IdK: a
              IdK: i
            IdK: t
          Assign
            IdK: i
            Op: +
              IdK: i
              ConstK: 1
  FuncK
    VoidK
    IdK: main
    ParamsK
      VoidK
    CompK
      Var_DeclK
        IntK
        IdK: i
      Assign
        IdK: i
        ConstK: 0
      While
        Op: <
          IdK: i
          ConstK: 10
        CompK
          Assign
            Arry_ElemK
              IdK: x
              IdK: i
            IdK: input
          Assign
            IdK: i
            Op: +
              IdK: i
              ConstK: 1
          CallK
            IdK: sort
            ArgsK
              IdK: x
              ConstK: 0
              ConstK: 10
          Assign
            IdK: i
            ConstK: 0
          While
            Op: <
              IdK: i
              ConstK: 10
            CompK
              CallK
                IdK: output
                ArgsK
                  Arry_ElemK
                    IdK: x
                    IdK: i
              Assign
                IdK: i
                Op: +
                  IdK: i
                  ConstK: 1
```
