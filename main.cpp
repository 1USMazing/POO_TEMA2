#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <string>
using namespace std;

/*1. toate clasele vor conține obligatoriu constructori de inițializare,
parametrizati și de copiere, destructor, iar operatorii >>,<<,=
să fie supraincarcati;
2. clasele derivate trebuie sa contina constructori prin care sa
se evidentieze transmiterea parametrilor către constructorul din
clasa de baza;
3. ilustrarea conceptelor de moștenire și functii virtuale
(pure – unde se considera mai natural);
4. tratarea exceptiilor;
5. citirea informațiilor complete a n obiecte, memorarea
și afisarea acestora.

Tema 1. liste de numere intregi, reprezentate ca tablouri
uni- si bidimensionale. Se dau urmatoarele clase:
- Vector (int dim, int *a)
- Matrice (Vector *v) – clasa abstracta
- Matrice_oarecare (int lin): Matrice
- Matrice_patratica (int dim): Matrice /*– nr de linii (dim)
trebuie sa coincida cu dimensiunea oricarui vector component

Clasa derivata trebuie sa contina o metoda care sa verifice
daca o matrice triunghiulara este diagonala.
Pentru matricile patratice, functia de afisare sa conțină și
determinantul acestora.*/

class Vector
{
protected: int dim, *a;

public:
    friend ostream& operator <<(ostream &out, const Vector &array);
    friend istream& operator >>(istream &in, Vector &array);
    friend class Matrice;
    friend class Matrice_patratica;
    friend class Matrice_oarecare;

    //Constructor de initializare
    Vector(int dimensiune=0)
    {
      dim=dimensiune;
      if(dimensiune>0)
          a=new int[dimensiune];
      else
        {
         a=NULL;
        }
    }

    //Constructor de copiere
     Vector(const Vector &tablou)
     {
      dim=tablou.dim;
      if(dim>0)
      {
      a=new int[dim];
      for(int i=0; i<dim; i++)
        a[i]=tablou.a[i];
      }
     }

    //Destructor
    ~Vector()
    {
        delete [] a;
    }

    Vector operator =(const Vector &v)
    {
       this->dim=v.dim;
       for(int i=0; i<dim; i++)
       {
           this->a[i]=v.a[i];
       }
    //Returnam referinta obiectului
       return *this;
    }

    int GetDimensiune()
    {
        return dim;
    }
};

ostream& operator <<(ostream &out, const Vector &array)
    {
        for(int i=0; i<array.dim; i++)
        {
            out<<array.a[i]<<" ";
        }
        return out;
    }

istream & operator >>(istream &in, Vector &array)
    {
        if(array.dim>0)
        {
            for(int i=0; i<array.dim; i++)
            {
                int element;

                try
                    {
                        if(!(in>>element)){throw element; cout<<"THROW!\n";}//Verificam daca elementul citit este diferit de tipul declarat
                    }
                    catch(int element){cout<<"Dati un numar intreg!\n"; exit(0);}

                array.a[i]=element;
            }
            return in;
        }
    }

//Clasa abstracta contine functie virtuala pura
class Matrice
{
protected: Vector *v;

public:

friend ostream& operator <<(ostream &out, const Matrice &mat);

    //Declarare destructor virtual pur care nu permite crearea unui obiect de tip Matrice
    virtual ~Matrice()=0;

    //functie virtuala pura ce este declarata atribuindu-se 0 in clasa abstracta
    virtual void afisare()=0;

    //Constructor de initializare
    Matrice(vector<Vector> &Matrice_linii)
    {
        //v va pointa adresa de memorie a vectorului Matrice_linii
        v=&Matrice_linii[0];

        for(int i=0; i<Matrice_linii.size(); i++)
            v[i]=Matrice_linii[i];
    }

    Matrice(Vector* mat)
    {
        v=mat;
    }

    //Constructor de copiere
    Matrice(Matrice &mat)
    {
        this->v=&mat.v[0];

        for(int i=0; v[i].dim>0; i++)
            this->v[i]=mat.v[i];
    }
};

//Implementare Destructor virtual pur
Matrice::~Matrice()
    {
        delete [] v;
    }

class Matrice_oarecare: public Matrice
{
protected:

    int lin;

public:

    friend ostream& operator << (ostream& out, const Matrice_oarecare& mat);

    //Constructor ce transmite parametrii spre clasa de baza
    Matrice_oarecare(vector<Vector> &Matrice_linii):Matrice(Matrice_linii)
    {
       lin=Matrice_linii.size();
    }

     //Constructor de copiere
    Matrice_oarecare(Matrice_oarecare &mat):Matrice(mat.v)
    {
        this->lin=mat.lin;

        this->v=&mat.v[0];

        for(int i=0; i<lin; i++)
            this->v[i]=mat.v[i];
    }

    void afisare()
    {
        cout<<"Matricea oarecare este"<<endl;
        for(int i=0; i<lin; i++)
            cout<<v[i]<<endl;
    }

    ~Matrice_oarecare()
    {
        lin=0;
    }

    int GetLinie()
    {
        return lin;
    }

    Vector* GetVector()
    {
        return v;
    }

    Matrice_oarecare operator =(const Matrice_oarecare &mat)
    {
       this->lin=mat.lin;

       this->v=&mat.v[0];

       for(int i=0; i<lin; i++)
           this->v[i]=mat.v[i];
    }
};

ostream& operator <<(ostream &out, Matrice_oarecare &mat)
    {
        out<<"Matricea oarecare este"<<endl;

        Vector *vect=mat.GetVector();

        for(int i=0; i<mat.GetLinie(); i++)
            out<<vect[i]<<endl;
        return out;
    }

class Matrice_patratica: public Matrice
{
    int dim;

public:

    friend ostream& operator <<(ostream &out, Matrice_patratica &mat);

    //Constructor ce transmite parametrii spre clasa de baza
    Matrice_patratica(vector<Vector> &Matrice_linii):Matrice(Matrice_linii)
    {
      dim=Matrice_linii.size();
    }

    //Constructor de copiere
    Matrice_patratica(Matrice_patratica &m):Matrice(m.v)
    {
        this->dim=m.dim;

        this->v=&m.v[0];

        for(int i=0; i<dim; i++)
            this->v[i]=m.v[i];
    }

    void diagonala()
    {
        int elemente_diagonala_nenule=0, elemente_nule=0;

        for(int i=0; i<dim; i++)
        {
            for(int j=0; j<dim; j++)
            {
                if(i==j && v[i].a[j] !=0) elemente_diagonala_nenule++;
                else if(i!=j && v[i].a[j]==0) elemente_nule++;
            }
        }
        if(elemente_diagonala_nenule==dim && elemente_nule==dim*dim-elemente_diagonala_nenule)
                cout<<"Matricea este triunghiulara! "<<endl;
    }

    void afisare()
    {
        cout<<"Matricea patratica este:"<<endl;
        for(int i=0; i<dim; i++)
            cout<<v[i]<<endl;

        vector<Vector>mat;

        for(int i=0; i<dim; i++)
            mat.push_back(v[i]);

        cout<<"Determinantul este: "<<Determinant(mat, dim)<<endl;
    }

    //Dezvoltarea dupa prima linie
    int Determinant(vector<Vector>matrice, int n)
    {
        if(n==1) return matrice[0].a[0];

        int D=0;
        int semn=1;

        for(int i=0; i<n; i++)
        {
            //Stabilim minorul fiecarui element de pe prima linie
            vector<Vector>submatrice=GetMinor(matrice, i, n);

            semn=pow(-1,i);
            D+=matrice[0].a[i]*semn*Determinant(submatrice, n-1);
        }

        return D;
    }

    vector<Vector> GetMinor(vector<Vector>matrice, int coloana, int n)
    {
        vector<Vector>submatrice;

        for(int i=1; i<n; i++)
           {
            vector<int>elemente(0);

            for(int j=0; j<n; j++)
                {
                    if(j !=coloana)
                        elemente.push_back(matrice[i].a[j]);
                }

                Vector vect(elemente.size());

                for(int k=0; k<elemente.size(); k++)
                    vect.a[k]=elemente[k];

                submatrice.push_back(vect);
           }

        return submatrice;
    }

    ~Matrice_patratica()
    {
        dim=0;
    }

    int GetDimensiune()
    {
        return dim;
    }

    Vector* GetVector()
    {
        return v;
    }

    Matrice_patratica operator =(const Matrice_patratica &mat)
    {
       this->dim=mat.dim;

       this->v=&mat.v[0];

       for(int i=0; i<mat.dim; i++)
          this->v[i]=mat.v[i];
    }
};

ostream& operator <<(ostream &out, Matrice_patratica &mat)
{
    out<<"Matricea patratica este:"<<endl;

    Vector* vect=mat.GetVector();
    int dim=mat.GetDimensiune();

        for(int i=0; i<dim; i++)
            out<<vect[i]<<endl;

        vector<Vector>matrice;

        for(int i=0; i<dim; i++)
            matrice.push_back(vect[i]);

        out<<"Determinantul este: "<<mat.Determinant(matrice, dim)<<endl;

    return out;
}

int main()
{
vector<Vector>Obiecte_Vector;

    int linie;
    cout<<"Numarul de vectori ce vor fi cititi: ";
    try
    {
        if(!(cin>>linie)){throw linie; cout<<"THROW!"<<endl;}
    }
    catch(int linie){cout<<"Dati un numar corect!"<<endl; return 0;}

    try
    {
        if(linie<=0){throw linie; cout<<"THROW!"<<endl;}
    }
    catch(int linie){cout<<"Numarul de vectori trebuie sa fie >0!"<<endl; return 0;}

    int dimensiune;
    cout<<"Dati dimensiunea vectorilor: ";

    try
    {
        if(!(cin>>dimensiune)){throw linie; cout<<"THROW!"<<endl;}
    }
    catch(int linie){cout<<"Dati un numar corect!"<<endl; return 0;}

    try
    {
        if(dimensiune<=0){throw dimensiune; cout<<"THROW!"<<endl; }
    }
    catch(int dimensiune){cout<<"Dimensiunea vectorilor trebuie sa fie >0!"<<endl; return 0;}

    for(int i=1; i<=linie; i++)
    {
        Vector v(dimensiune);
        cin>>v;
        if(v.GetDimensiune()<dimensiune) return 0;
        Obiecte_Vector.push_back(v);
    }

/*for(int i=0; i<Obiecte_Vector.size(); i++)
    cout<<Obiecte_Vector[i]<<endl;*/

if(linie==dimensiune)
{
Matrice_patratica M(Obiecte_Vector);
M.diagonala();
cout<<M;

/*Matrice_patratica N(M);
N.diagonala();
cout<<N;
N.afisare();*/
}

else
{
    Matrice_oarecare M(Obiecte_Vector);
    cout<<M;

    /*Matrice_oarecare N=M;
    cout<<N;
    N.afisare();*/
}

}
