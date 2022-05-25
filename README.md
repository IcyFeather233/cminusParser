# cminusParser

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
