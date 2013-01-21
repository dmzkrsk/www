//
//  Copyright (c) Cail Lomecb (Igor Ruskih) 1999-2000 <ruiv@uic.nnov.ru>
//  You can use, modify, distribute this code or any other part
//  of this program in sources or in binaries only according
//  to License (see /doc/(rus/)?license.txt for more information).
//
//  Clear Sgml Parser
//
#include "stdafx.h"
#include<string.h>
#include<stdio.h>

#include "sgml.h"

bool get_number(char *str, double *res);
bool isspace(char c);

CSgmlEl::CSgmlEl()
{
  eparent= 0;
  enext  = 0;
  eprev  = 0;
  echild = 0;
  chnum  = 0;
  type   = eBaseEl;

  name   = 0;
  content= 0;
  contentsz = 0;
  parnum = 0;
};
CSgmlEl::~CSgmlEl()
{
  if ((type == eBaseEl)&&enext) enext->destroylevel(); //My Editings
  if (echild) echild->destroylevel();
  if (name) delete[] name;
  if (content) delete[] content;
  for (int i=0;i < parnum;i++){
    if (params[i][0]) delete params[i][0];
    if (params[i][1]) delete params[i][1];
  };
};
PSgmlEl CSgmlEl::createnew(ElType type, PSgmlEl parent, PSgmlEl after)
{
  PSgmlEl El = new CSgmlEl;
  El->type = type;
  if (parent){
    El->enext = parent->echild;
    El->eparent = parent;
    parent->echild->eprev = El;
    parent->echild = El;
    parent->chnum++;
    parent->type = eBlockedEl;
  }else
    if (after) after->insert(El);
  return El;
};
bool CSgmlEl::init()
{
  return true;
};

bool CSgmlEl::parse(const char *src,int sz)
{
PSgmlEl Child, Parent, Next = 0;
int i, j, lins, line;
int ls,le,rs,re;

  // start object - base
  type = eBaseEl;
  Next = this;

  lins = line = 0;
  for (i = 0; i < sz; i++){
    if (i >= sz) continue;

    // comments
    if ( *((int*)(src+i)) == '--!<' && i+4 < sz){
      i += 4;
      while((src[i] != '-' || src[i+1] != '-' || src[i+2] != '>') && i+3 < sz) i++;
      i+=3;
    };
    line = i;

    if ( src[i] == '<' || i == sz-1){
      while(line > lins){
        // linear
        j = lins;
        while(isspace(src[j]) && j < i){
          j++;
        };
        if(j == i) break; // empty text
        Child = createnew(ePlainEl,0,Next);
        Child->init();
        Child->setcontent((char*)src + lins, i - lins);
        Next = Child;
        break;
      };
      if (i == sz-1) continue;
      // start or single tag
      if (src[i+1] != '/'){
        Child = createnew(eSingleEl,NULL,Next);
        Next  = Child;
        Child->init();
        j = i+1;
        while (src[i] != '>' && !isspace(src[i]) && i < sz) i++;
        Child->name = new char[i-j+1];
        strncpy(Child->name, src+j, i-j);
        Child->name[i-j] = 0;
        // parameters
        Child->parnum = 0;
        while(src[i] != '>' && Child->parnum < MAXPARAMS && i < sz){
          ls = i;
          while (isspace(src[ls]) && ls < sz) ls++;
          le = ls;
          while (!isspace(src[le]) && src[le]!='>' && src[le]!='=' && le < sz) le++;
          rs = le;
          while (isspace(src[rs]) && rs < sz) rs++;
          if (src[rs] == '='){
            rs++;
            while (isspace(src[rs]) && rs < sz) rs++;
            re = rs;
            if (src[re] == '"'){
              while(src[++re] != '"' && re < sz);
              rs++;
              i = re+1;
            }else if (src[re] == '\''){
              while(src[++re] != '\'' && re < sz);
              rs++;
              i = re+1;
            }else{
              while(!isspace(src[re]) && src[re] != '>' && re < sz) re++;
              i = re;
            };
          }else
            i = re = rs;

          if (ls == le) continue;
          if (rs == re){
            rs = ls;
            re = le;
          };
          int pn = Child->parnum;
          Child->params[pn][0] = new char[le-ls+1];
          strncpy(Child->params[pn][0], src+ls, le-ls);
          Child->params[pn][0][le-ls] = 0;
          Child->params[pn][1] = new char[re-rs+1];
          strncpy(Child->params[pn][1], src+rs, re-rs);
          Child->params[pn][1][re-rs] = 0;
          Child->parnum++;
          //substqoutes("&lt;",'<');
          //substqoutes("&gt;",'>');
        };
        lins = i+1;
      }else{  // end tag
        j = i+2;
        i+=2;
        while (src[i] != '>' && !isspace(src[i]) && i < sz) i++;
        int cn = 0;
        for(Parent = Next; Parent->eprev; Parent = Parent->eprev, cn++){
          if(!Parent->name) continue;
          int len = strlen(Parent->name);
          if (len != i-j) continue;
          if (Parent->type != eSingleEl ||
            strnicmp(src+j, Parent->name, len)) continue;
          break;
        };
        if(Parent && Parent->eprev){
          Parent->echild = Parent->enext;
          Parent->chnum = cn;
          Parent->type = eBlockedEl;
          Child = Parent->echild;
          if (Child) Child->eprev = 0;
          while(Child){
            Child->eparent = Parent;
            Child = Child->enext;
          };
          Parent->enext = 0;
          Next = Parent;
        };
        while(src[i] != '>' && i < sz) i++;
        lins = i+1;
      };
    };
  };
////
  return true;
};

bool CSgmlEl::setcontent(char *src,int sz)
{
  content = new char[sz+1];
  memmove(content,src,sz);
  content[sz]=0;
  contentsz = sz;
  return true;
};

void CSgmlEl::insert(PSgmlEl El)
{
  El->eprev = this;
  El->enext = this->enext;
  El->eparent = this->eparent;
  if (this->enext) this->enext->eprev = El;
  this->enext = El;
};

void CSgmlEl::destroylevel()
{
  if (enext) enext->destroylevel();
  delete this;
};

PSgmlEl CSgmlEl::parent()
{
  return eparent;
};
PSgmlEl CSgmlEl::next()
{
  return enext;
};
PSgmlEl CSgmlEl::prev()
{
  return eprev;
};
PSgmlEl CSgmlEl::child()
{
  return echild;
};
ElType  CSgmlEl::gettype()
{
  return type;
};


char *CSgmlEl::GetChrParam(char *par)
{
  for (int i=0; i < parnum; i++)
    if (!stricmp(par,params[i][0])){
      return params[i][1];
    };
  return 0;
};

bool CSgmlEl::GetIntParam(char *par,int *result)
{
double res;
  for (int i=0; i < parnum; i++)
    if (!stricmp(par,params[i][0])){
      bool b = get_number(params[i][1],&res);
      *result = (int)res;
      if (!b) *result = 0;
      return b;
    };
  return false;
};

bool CSgmlEl::GetFltParam(char *par, double *result)
{
double res;
  for (int i = 0; i < parnum; i++)
    if (!stricmp(par,params[i][0])){
      bool b = get_number(params[i][1],&res);
      *result = (double)res;
      if (!b) *result = 0;
      return b;
    };
  return false;
};

PSgmlEl CSgmlEl::search(char *TagName)
{
PSgmlEl Next = this->enext;
  while(Next){
    if (!stricmp(TagName,Next->name)) return Next;
    Next = Next->enext;
  };
  return Next;
};

PSgmlEl CSgmlEl::enumchilds(int no)
{
PSgmlEl El = this->echild;
  while(no && El){
    El = El->enext;
    no--;
  };
  return El;
};

PSgmlEl CSgmlEl::fprev()
{
PSgmlEl El = this;
  if (!El->eprev) return El->eparent;
  if (El->eprev->echild)
    return El->eprev->echild->flast();
  return El->eprev;
};
PSgmlEl CSgmlEl::fnext()
{
PSgmlEl El = this;
  if (El->echild) return El->echild;
  while(!El->enext){
    El = El->eparent;
    if (!El) return 0;
  };
  return El->enext;
};
PSgmlEl CSgmlEl::ffirst()
{
PSgmlEl Prev = this;
  while(Prev){
    if (!Prev->eprev) return Prev;
    Prev = Prev->eprev;
  };
  return Prev;
};

PSgmlEl CSgmlEl::flast()
{
PSgmlEl Nxt = this;
  while(Nxt->enext || Nxt->echild){
    if (Nxt->enext){
      Nxt = Nxt->enext;
      continue;
    };
    if (Nxt->echild){
      Nxt = Nxt->echild;
      continue;
    };
  };
  return Nxt;
}

/////////////////////////////// CSgmlEdit //////////////////////////////////

CSgmlEdit::CSgmlEdit(){ };
CSgmlEdit::~CSgmlEdit()
{
  if (eparent){
    eparent->chnum--;
    if (eparent->echild == this) eparent->echild = enext;
  };
  if (enext) enext->eprev = eprev;
  if (eprev) eprev->enext = enext;
};

void CSgmlEdit::setname(char *newname)
{
  if (name){
    delete name;
    name = 0;
  };
  if (newname){
    name = new char[strlen(newname)+1];
    strcpy(name,newname);
  };
};
bool CSgmlEdit::addparam(char *name, char *val)
{
int i;
  if (parnum == MAXPARAMS) return false;
  i = parnum;
  parnum++;
  for (i = 0; i < parnum - 1; i++)
    if (!stricmp(params[i][0],name)){
      delete params[i][0];
      delete params[i][1];
      parnum--;
      break;
    };
  params[i][0] = new char[strlen(name)+1];
  params[i][1] = new char[strlen(val)+1];
  strcpy(params[i][0], name);
  strcpy(params[i][1], val);
  return true;
};
bool CSgmlEdit::addparam(char *name, int val)
{
char IntVal[20];
  sprintf(IntVal,"%d",val);
  return addparam(name,IntVal);
};
bool CSgmlEdit::addparam(char *name, double val)
{
char FltVal[20];
  sprintf(FltVal,"%.2f",val);
  return addparam(name,FltVal);
};
bool CSgmlEdit::delparam(char *name)
{
  for (int i = 0; i < parnum; i++)
    if (!stricmp(params[i][0],name)){
      delete params[i][0];
      delete params[i][1];
      params[i][0] = params[parnum-1][0];
      params[i][1] = params[parnum-1][1];
      parnum--;
      return true;
    };
  return false;
};
bool CSgmlEdit::changecontent(char *data, int len)
{
  if (type != ePlainEl) return false;
  if (content) delete[] content;
  content = new char[len];
  memmove(content, data, len);
  contentsz = len;
  return true;
};

bool CSgmlEdit::isloop(PSgmlEdit El, PSgmlEdit Parent)
{
  while(Parent){
    if (El == Parent) return true;
    Parent = (PSgmlEdit)Parent->eparent;
  };
  return false;
};
bool CSgmlEdit::move(PSgmlEdit parent, PSgmlEdit after)
{
  if (isloop(this,parent)) return false;
  if (after && isloop(this,(PSgmlEdit)after->eparent)) return false;
  if (after){
    if (enext) enext->eprev = eprev;
    if (eprev) eprev->enext = enext;
    if (this->eparent->echild == this)
      this->eparent->echild = this->enext;
    this->eparent->chnum--;

    after->insert(this);
    this->eparent = after->eparent;
    if (this->eparent) this->eparent->chnum++;
    return true;
  }else
  if (parent){
    if (enext) enext->eprev = eprev;
    if (eprev) eprev->enext = enext;
    if (this->eparent->echild == this)
      this->eparent->echild = this->enext;
    this->eparent->chnum--;
    this->eparent = parent;
    enext = parent->echild;
    eprev = 0;
    this->eparent->echild = this;
    this->eparent->chnum++;
    if (enext) enext->eprev = this;
    return true;
  };
  return false;
};

int CSgmlEdit::getlevelsize(int Lev)
{
int Pos = 0;
PSgmlEdit tmp = this;
  do{
    if (tmp->gettype() != ePlainEl)
      Pos +=Lev*SP;
    if (tmp->name)
      Pos += strlen(tmp->name)+1;
    for (int i = 0;i < tmp->parnum;i++){
      Pos +=strlen(tmp->params[i][0])+2;
      Pos +=strlen(tmp->params[i][1])+2;
    };
    if (tmp->name) Pos +=3;
    if (tmp->gettype() == ePlainEl && tmp->content)
      Pos += strlen(tmp->content)+2;
    if (tmp->echild)
      Pos += PSgmlEdit(tmp->echild)->getlevelsize(Lev+1);
    if (tmp->gettype() == eBlockedEl && tmp->name){
      Pos += Lev*SP+5;
      Pos += strlen(tmp->name);
    };
    tmp = (PSgmlEdit)tmp->enext;
  }while(tmp);
  return Pos;
};

int CSgmlEdit::savelevel(char *Dest,int Lev)
{
int i,Pos = 0;
PSgmlEdit tmp = this;
  do{
    if (tmp->gettype() != ePlainEl)
      for(i = 0; i < Lev*SP; i++)
        Pos += sprintf(Dest+Pos," ");
    if (tmp->name)
      Pos += sprintf(Dest+Pos,"<%s",tmp->name);
    for (i = 0; i < tmp->parnum; i++){
      Pos += sprintf(Dest+Pos," %s=",tmp->params[i][0]);
      Pos += sprintf(Dest+Pos,"\"%s\"",tmp->params[i][1]);
    }
    if (tmp->name)
      Pos += sprintf(Dest+Pos,">\r\n");
    if (tmp->gettype() == ePlainEl)
      Pos += sprintf(Dest+Pos,"%s\r\n", tmp->content);
    if (tmp->echild)
      Pos += PSgmlEdit(tmp->echild)->savelevel(Dest+Pos,Lev+1);
    if (tmp->gettype() == eBlockedEl){
      for(i = 0; i < Lev*SP; i++)
        Pos += sprintf(Dest+Pos," ");
      Pos += sprintf(Dest+Pos,"</");
      if (tmp->name) Pos += sprintf(Dest+Pos,"%s",tmp->name);
      Pos += sprintf(Dest+Pos,">\r\n");
    };
    tmp = (PSgmlEdit)tmp->enext;
  }while(tmp);
  return Pos;
};
//bool CSgmlEdit::SetChild(PSgmlEl Child)
//{
//};
//bool CSgmlEdit::InsertMe(PSgmlEl Parent,PSgmlEl After);//???

#include<math.h>
#include<float.h>

bool isspace(char c)
{
  if (c==0x20 || c=='\t' || c=='\r' || c=='\n') return true;
  return false;
};


bool get_number(char *str, double *res)
{
double Numr, r, flt;
int    pos, Type, Num;
int    s, e, i, j, pt, k, ExpS, ExpE;
bool   Exp = false, ExpSign = true;

pos = strlen(str);

s = 0;e = pos;
Type = 3;
do{
  if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X')){
    s = 2;
    Type = 0;
    break;
  };
  if (str[0] == '$' || str[0] == '#'){
    s = 1;
    Type = 0;
    break;
  };
  break;
}while(1);

switch(Type){
  case 0:
    Num = 0;
    i = e-1;
    while(i >= s){
      j = str[i];
      if(((j < 0x30) || (j > 0x39)) &&
        (((j | 0x20) < 'a') || ((j | 0x20) > 'f')))
          return false;
      if(j > 0x40) j -=7;
      j &=15;
      if(i > e-9) Num |= (j << ((e-i-1)*4) );
      i--;
    };
    *res = (int)Num;
    break;
  case 3:
    for(i = s;i < e;i++)
    if (str[i] == 'e' || str[i] =='E'){
      Exp = true;
      ExpS = i+1;
      if (str[i+1] == '+' || str[i+1] == '-'){
        ExpS++;
        if (str[i+1] == '-') ExpSign = false;
      };
      ExpE = e;
      e = i;
    };
    pt = e;
    for(i = s;i < e;i++)
      if (str[i] == '.'){
        pt = i;
        break;
      };
    Numr = 0;
    i = pt-1;
    while(i >= s){
      j = str[i];
      if((j < 0x30)||(j > 0x39))
        return false;
      j &=15;
      k = pt-i-1;
      r = (long double)j;
      while(k){
        k--;
        r *=10;
      };
      Numr += r;
      i--;
    };
    i = e-1;
    while(i > pt){
      j = str[i];
      if((j < 0x30)||(j > 0x39))
        return false;
      j &=15;
      k = i-pt;
      r = 0;
      r = j;
      while(k){
        k--;
        r /=10;
      };
      Numr += r;
      i--;
    };
    if (Exp){
      flt = 0;
      i = ExpE-1;
      while(i >= ExpS){
        j = str[i];
        if((j < 0x30)||(j > 0x39))
          return false;
        j &=15;
        k = ExpE-i-1;
        r = (long double)j;
        while(k){
          k--;
          r *=10;
        };
        flt += r;
        i--;
      };
      if (ExpSign)  Numr = Numr*pow(10,flt);
      if (!ExpSign) Numr = Numr/pow(10,flt);
    };
    *res = Numr;
    break;
};
return true;
};
