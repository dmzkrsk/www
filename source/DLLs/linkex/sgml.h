//
//  Copyright (c) Cail Lomecb (Igor Ruskih) 1999-2000 <ruiv@uic.nnov.ru>
//  You can use, modify, distribute this code or any other part
//  of this program in sources or in binaries only according
//  to License (see /doc/(rus/)?license.txt for more information).
//
#ifndef _CSgml_
#define _CSgml_

typedef class CSgmlEl *PSgmlEl;
typedef class CSgmlEdit *PSgmlEdit;
typedef char  *TParams[2];

#define MAXPARAMS 0x30
#define SP 1

enum ElType{
  eBaseEl, eBlockedEl, eSingleEl, ePlainEl
};

class CSgmlEl
{
protected:
  PSgmlEl eparent;
  PSgmlEl enext;
  PSgmlEl eprev;
  PSgmlEl echild;
  int     chnum;
  ElType  type;

  //for derived classes
  virtual PSgmlEl createnew(ElType type, PSgmlEl parent, PSgmlEl after);
  virtual bool init();

  void destroylevel();
  void insert(PSgmlEl El);
  bool setcontent(char *src,int sz);
  friend class CSgmlEdit;

public:
  char   *name;
  char   *content;
  int     contentsz;
  TParams params[MAXPARAMS];
  int     parnum;

  CSgmlEl();
  ~CSgmlEl();
  bool parse(const char *src,int sz);

  virtual PSgmlEl parent();
  virtual PSgmlEl next();
  virtual PSgmlEl prev();
  virtual PSgmlEl child();
  ElType  gettype();

  char	*GetChrParam(char *par);
  bool   GetIntParam(char *par,int *result);
  bool   GetFltParam(char *par,double *result);

  PSgmlEl search(char *TagName);
  PSgmlEl enumchilds(int no);
  // in full hierarchy
  virtual PSgmlEl fprev();
  virtual PSgmlEl fnext();
  virtual PSgmlEl ffirst();
  virtual PSgmlEl flast();
};

class CSgmlEdit:public CSgmlEl
{
protected:
  bool  isloop(PSgmlEdit El, PSgmlEdit Parent);
public:
  CSgmlEdit();
  ~CSgmlEdit();

  void setname(char *newname);

  // parameters add and change
  bool addparam(char *name, char *val);
  bool addparam(char *name, int val);
  bool addparam(char *name, double val);
  bool changecontent(char *data, int len);
  bool delparam(char *name);

  bool move(PSgmlEdit parent, PSgmlEdit after);
  PSgmlEdit copytree(PSgmlEdit el);

  // saving tree into text
  int  getlevelsize(int lev);
  int  savelevel(char *dest,int lev);
};

#endif