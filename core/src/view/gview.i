%module gview
%{
#include <gicoreview.h>
#include <gicanvas.h>
#include <giview.h>
#include <mgstorage.h>
%}

%include <mgvector.h>
%template(Ints) mgvector<int>;

%include <gicanvas.h>
%include <giview.h>
%include <mgstorage.h>
%include <gigesture.h>
%include <gicolor.h>
%include <gicontxt.h>
%include <gicoreview.h>
