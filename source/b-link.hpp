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
        int *key,size;
        Nodo **ptr;

        Nodo(){
            key = new int[B];
            ptr=new Nodo* [B+1];
        }
    };
    Nodo *root;
 public:
  typedef Type data_type;

  BLinkTree() {root=nullptr;}

  ~BLinkTree() {}

  std::size_t size() const {}

  bool empty() const {
      if(root==NULL) {
          return 1;
      }else{
          return 0;
      }
  }

  bool search(const data_type& value) const {
      if(empty()) {
          cout << "El arbol esta vacio" << endl;
          return 0;
      }
      else{
          Nodo *cursor=root;
          while(cursor->hoja==false){
              for(int i=0;i<cursor->size;i++){
                  if(value<cursor->key[i]){
                      cursor=cursor->ptr[i];
                      break;
                  }
                  if(i==cursor->size-1){
                      cursor=cursor->ptr[i+1];
                      break;
                  }
              }
          }
          for(int i=0;i<cursor->size;i++){
              if(cursor->key[i]==value){
                  cout<<"Encontrado"<<endl;
                  return 1;
              }
          }
          cout<<"No encontrado";
          return 0;
      }
  }

  void insert(const data_type& value) {
      if (empty()) {
          root = new Nodo;
          root->key[0] = value;
          root->hoja = true;
          root->size = 1;
      } else {
          Nodo *cursor = root;
          Nodo *parent;
          while (cursor->hoja == false) {
              parent = cursor;
              for (int i = 0; i < cursor->size; i++) {
                  if (value < cursor->key[i]) {
                      cursor = cursor->ptr[i];
                      break;
                  }
                  if (i == cursor->size - 1) {
                      cursor = cursor->ptr[i + 1];
                      break;
                  }
              }
          }
          if (cursor->size < B) {
              int i = 0;
              while (value > cursor->key[i] && i < cursor->size)
                  i++;
              for (int j = cursor->size; j > i; j--) {
                  cursor->key[j] = cursor->key[j - 1];
              }
              cursor->key[i] = value;
              cursor->size++;
              cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
              cursor->ptr[cursor->size - 1] = NULL;
          } else {
              Nodo *newLeaf = new Nodo;
              int virtualNode[B + 1];
              for (int i = 0; i < B; i++) {
                  virtualNode[i] = cursor->key[i];
              }
              int i = 0, j;
              while (value > virtualNode[i] && i < B)
                  i++;
              for (int j = B + 1; j > i; j--) {
                  virtualNode[j] = virtualNode[j - 1];
              }
              virtualNode[i] = value;
              newLeaf->hoja = true;
              cursor->size = (B + 1) / 2;
              newLeaf->size = B + 1 - (B + 1) / 2;
              cursor->ptr[cursor->size] = newLeaf;
              newLeaf->ptr[newLeaf->size] = cursor->ptr[B];
              cursor->ptr[B] = NULL;
              for (i = 0; i < cursor->size; i++) {
                  cursor->key[i] = virtualNode[i];
              }
              for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
                  newLeaf->key[i] = virtualNode[j];
              }
              if (cursor == root) {
                  Nodo *newRoot = new Nodo;
                  newRoot->key[0] = newLeaf->key[0];
                  newRoot->ptr[0] = cursor;
                  newRoot->ptr[1] = newLeaf;
                  newRoot->hoja = false;
                  newRoot->size = 1;
                  root = newRoot;
              } else {
                  insertInternal(newLeaf->key[0], parent, newLeaf);
              }
          }
      }
  }
  void posactual(){

  }

  void insertInternal(int x, Nodo *cursor, Nodo *child) {
      if (cursor->size < B) {
          int i = 0;
          while (x > cursor->key[i] && i < cursor->size)
              i++;
          for (int j = cursor->size; j > i; j--) {
              cursor->key[j] = cursor->key[j - 1];
          }
          for (int j = cursor->size + 1; j > i + 1; j--) {
              cursor->ptr[j] = cursor->ptr[j - 1];
          }
          cursor->key[i] = x;
          cursor->size++;
          cursor->ptr[i + 1] = child;
      } else {
          Nodo *newInternal = new Nodo;
          int virtualKey[B + 1];
          Nodo *virtualPtr[B + 2];
          for (int i = 0; i < B; i++) {
              virtualKey[i] = cursor->key[i];
          }
          for (int i = 0; i < B + 1; i++) {
              virtualPtr[i] = cursor->ptr[i];
          }
          int i = 0, j;
          while (x > virtualKey[i] && i < B)
              i++;
          for (int j = B + 1; j > i; j--) {
              virtualKey[j] = virtualKey[j - 1];
          }
          virtualKey[i] = x;
          for (int j = B + 2; j > i + 1; j--) {
              virtualPtr[j] = virtualPtr[j - 1];
          }
          virtualPtr[i + 1] = child;
          newInternal->hoja = false;
          cursor->size = (B + 1) / 2;
          newInternal->size = B - (B + 1) / 2;
          for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
              newInternal->key[i] = virtualKey[j];
          }
          for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
              newInternal->ptr[i] = virtualPtr[j];
          }
          if (cursor == root) {
              Nodo *newRoot = new Nodo;
              newRoot->key[0] = cursor->key[cursor->size];
              newRoot->ptr[0] = cursor;
              newRoot->ptr[1] = newInternal;
              newRoot->hoja = false;
              newRoot->size = 1;
              root = newRoot;
          } else {
              insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
          }
      }
  }

  Nodo *findParent(Nodo *cursor, Nodo *child) {
      Nodo *parent;
      if (cursor->hoja || (cursor->ptr[0])->hoja) {
          return NULL;
      }
      for (int i = 0; i < cursor->size + 1; i++) {
          if (cursor->ptr[i] == child) {
              parent = cursor;
              return parent;
          } else {
              parent = findParent(cursor->ptr[i], child);
              if (parent != NULL)
                  return parent;
          }
      }
      return parent;
  }
  void remove(const data_type& value) {}

 private:

};

}  // namespace Concurrent
}  // namespace EDA

#endif  // SOURCE_B_LINK_HPP_
