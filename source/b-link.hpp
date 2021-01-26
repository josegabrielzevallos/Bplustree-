// Copyright
#ifndef SOURCE_B_LINK_HPP_
#define SOURCE_B_LINK_HPP_

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

using namespace std;

namespace EDA {
namespace Concurrent {


template <std::size_t B, typename Type>
class BLinkTree {
    struct Nodo{
        bool hoja;
        int *key,Tam;
        Nodo **ptr;

        Nodo(){
            key = new int[B];
            ptr=new Nodo* [B+1];
        }
    };
    Nodo *root;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
  typedef Type data_type;

  BLinkTree() {root=nullptr;}

  ~BLinkTree() {}

  std::size_t size() const {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool empty() const {
      if(root==NULL) {
          return 1;
      }else{
          return 0;
      }
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  bool search(const data_type& value)  {
      if(empty()) {
          cout << "El arbol esta vacio" << endl;
          return 0;
      }
      else{
          Nodo *apuntador=root;
          Nodo *padre;
          actual(value,apuntador,padre);

          for(int i=0;i<apuntador->Tam;i++){
              if(apuntador->key[i]==value){
                  cout<<"Encontrado"<<endl;
                  return 1;
              }
          }
          cout<<"No encontrado";
          return 0;
      }
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void insert(const data_type& value) {
      if (empty()) {
          root = new Nodo;
          root->key[0] = value;//inicilaizamos el rooty la hoja
          root->hoja = true;// es un ahoja
          root->Tam = 1;
      } else {

          Nodo *apuntador = root;
          Nodo *padre;
          actual(value,apuntador,padre);

          if (apuntador->Tam < B) {
              int i = 0;
              mayormenor(value,apuntador,i);
              //int i = 0;

              apuntador->key[i] = value;
              apuntador->Tam++;
              apuntador->ptr[apuntador->Tam] = apuntador->ptr[apuntador->Tam - 1];
              apuntador->ptr[apuntador->Tam - 1] = NULL;
          } else {
              Nodo *nuevahoja = new Nodo;// se crea un anueva hoja
              int tempNodo[B + 1];
              for (int i = 0; i < B; i++) {
                  tempNodo[i] = apuntador->key[i];
              }
              int i = 0, j;

              while (value > tempNodo[i] && i < B)
                  i++;
              for (int j = B + 1; j > i; j--) {
                  tempNodo[j] = tempNodo[j - 1];
              }

              tempNodo[i] = value;
              nuevahoja->hoja = true;
              apuntador->Tam = (B + 1) / 2;
              nuevahoja->Tam = B + 1 - (B + 1) / 2;
              apuntador->ptr[apuntador->Tam] = nuevahoja;
              nuevahoja->ptr[nuevahoja->Tam] = apuntador->ptr[B];
              apuntador->ptr[B] = NULL;

              for (i = 0; i < apuntador->Tam; i++) {
                  apuntador->key[i] = tempNodo[i];
              }
              for (i = 0, j = apuntador->Tam; i < nuevahoja->Tam; i++, j++) {
                  nuevahoja->key[i] = tempNodo[j];
              }
              if (apuntador == root) {
                  Nodo *newRoot = new Nodo;
                  newRoot->key[0] = nuevahoja->key[0];
                  newRoot->ptr[0] = apuntador;
                  newRoot->ptr[1] = nuevahoja;
                  newRoot->hoja = false;
                  newRoot->Tam = 1;
                  root = newRoot;
              } else {
                  Insertardentro(nuevahoja->key[0], padre, nuevahoja);
              }
          }
      }
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void remove(const data_type& value) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void mayormenor(const data_type& value,Nodo*& apuntador,int& indice){
      //int i = 0;
      //while (value > apuntador->key[i]){
      while (value > apuntador->key[indice] && indice < apuntador->Tam)//si el valor es mayor
          indice++;
      for (int j = apuntador->Tam; j > indice; j--) {// si es menor
          apuntador->key[j] = apuntador->key[j - 1];
      }
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void actual(const data_type& value,Nodo*& apuntador,Nodo*& padre){
      while (apuntador->hoja == false) {
          padre = apuntador;
          for (int i = 0; i < apuntador->Tam; i++) {
              if (value < apuntador->key[i]) {
                  apuntador = apuntador->ptr[i];
                  break;
              }
              if (i == apuntador->Tam - 1) {
                  apuntador = apuntador->ptr[i + 1];
                  break;
              }
          }
      }
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void Insertardentro(const data_type& value, Nodo *apuntador, Nodo *hijo) {
      if (apuntador->Tam < B) {
          int i = 0;

          mayormenor(value,apuntador,i);

          for (int j = apuntador->Tam + 1; j > i + 1; j--) {
              apuntador->ptr[j] = apuntador->ptr[j - 1];
          }
          apuntador->key[i] = value;
          apuntador->Tam++;
          apuntador->ptr[i + 1] = hijo;
      } else {
          Nodo *newInternal = new Nodo;
          int TempKey[B + 1];
          Nodo *virtualPtr[B + 2];
          for (int i = 0; i < B; i++) {
              TempKey[i] = apuntador->key[i];
          }
          for (int i = 0; i < B + 1; i++) {
              virtualPtr[i] = apuntador->ptr[i];
          }
          int i = 0, j;
          while (value > TempKey[i] && i < B)
              i++;
          for (int j = B + 1; j > i; j--) {
              TempKey[j] = TempKey[j - 1];
          }
          TempKey[i] = value;
          for (int j = B + 2; j > i + 1; j--) {
              virtualPtr[j] = virtualPtr[j - 1];
          }
          virtualPtr[i + 1] = hijo;
          newInternal->hoja = false;
          apuntador->Tam = (B + 1) / 2;
          newInternal->Tam = B - (B + 1) / 2;
          for (i = 0, j = apuntador->Tam + 1; i < newInternal->Tam; i++, j++) {
              newInternal->key[i] = TempKey[j];
          }
          for (i = 0, j = apuntador->Tam + 1; i < newInternal->Tam + 1; i++, j++) {
              newInternal->ptr[i] = virtualPtr[j];
          }
          if (apuntador == root) {
              Nodo *newRoot = new Nodo;
              newRoot->key[0] = apuntador->key[apuntador->Tam];
              newRoot->ptr[0] = apuntador;
              newRoot->ptr[1] = newInternal;
              newRoot->hoja = false;
              newRoot->Tam = 1;
              root = newRoot;
          } else {
              Insertardentro(apuntador->key[apuntador->Tam], Encontrarpadre(root, apuntador), newInternal);
          }
      }
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  Nodo *Encontrarpadre(Nodo *apuntador, Nodo *hijo) {
      Nodo *padre;
      if (apuntador->hoja || (apuntador->ptr[0])->hoja) {
          return NULL;
      }
      for (int i = 0; i < apuntador->Tam + 1; i++) {
          if (apuntador->ptr[i] == hijo) {
              padre = apuntador;
              return padre;
          } else {
              padre = Encontrarpadre(apuntador->ptr[i], hijo);
              if (padre != NULL)
                  return padre;
          }
      }
      return padre;
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 private:

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}  // namespace Concurrent
}  // namespace EDA

#endif  // SOURCE_B_LINK_HPP_
