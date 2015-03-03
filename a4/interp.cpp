// $Id: interp.cpp,v 1.18 2015-02-19 16:50:37-08 - - $

#include <memory>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"           , &interpreter::make_text           },
   {"ellipse"        , &interpreter::make_ellipse        },
   {"circle"         , &interpreter::make_circle         },
   {"polygon"        , &interpreter::make_polygon        },
   {"rectangle"      , &interpreter::make_rectangle      },
   {"square"         , &interpreter::make_square         },
   {"diamond"        , &interpreter::make_diamond        },
   {"triangle"       , &interpreter::make_triangle       },
   {"right_triangle" , &interpreter::make_right_triangle },
   {"isosceles"      , &interpreter::make_isosceles      },
   {"equilateral"    , &interpreter::make_equilateral    },
   //{"border"         , &interpreter::make_border         },
};
static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
 };  

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}

void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string name = *begin;
   objmap.emplace (name, make_shape (++begin, end));
   //for (; begin != end; ++begin) //del
      //cout << *begin << "\t"; //del
   //cout << endl; //del
}


void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such shape");
   }
   rgbcolor color {begin[0]};
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   object obj = object(itor->second, where, color);
   window::push_back(obj);
   //itor->second->draw (where, color);
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   string str;
   auto itor = fontcode.find(type);
   if (itor == fontcode.end()) {
      throw runtime_error (type + ": no such font");
   }
   void * font = itor->second;
   while (begin != end)
	str += (*begin++) + " ";
   //cout << str << endl;
   return make_shared<text> (font, str);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   return make_shared<ellipse> (from_string<GLfloat>(*begin),
			     from_string<GLfloat>(*(--end)));
}

shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   return make_shared<circle> (from_string<GLfloat>(*begin));
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vl;
   vertex vtx;
   int flipflop = true;
   for ( ; begin != end; ++begin) {
      if(flipflop) {
	 vtx.xpos = from_string<GLfloat>(*begin);
	 flipflop = not flipflop;
      }
      else {
	 vtx.ypos = from_string<GLfloat>(*begin);
	 vl.push_back(vtx);
	 flipflop = not flipflop;
      }
   }
   return make_shared<polygon> (vl);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vl;
   vertex rect[4];
   rect[0].xpos = 0;
   rect[0].ypos = 0;
   rect[1].xpos = 0;
   rect[1].ypos = from_string<GLfloat>(*(--end));
   rect[2].xpos = from_string<GLfloat>(*begin);
   rect[2].ypos = from_string<GLfloat>(*end);
   rect[3].xpos = from_string<GLfloat>(*begin);
   rect[3].ypos = 0;
   for (size_t i = 0; i < 4; ++i)
      vl.push_back(rect[i]);
   return make_shared<rectangle> (vl);
}

shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vl;
   vertex rect[4];
   rect[0].xpos = 0;
   rect[0].ypos = 0;
   rect[1].xpos = 0;
   rect[1].ypos = from_string<GLfloat>(*begin);
   rect[2].xpos = from_string<GLfloat>(*begin);
   rect[2].ypos = from_string<GLfloat>(*begin);
   rect[3].xpos = from_string<GLfloat>(*begin);
   rect[3].ypos = 0;
   for (size_t i = 0; i < 4; ++i)
      vl.push_back(rect[i]);
   return make_shared<square> (vl);
}

shape_ptr interpreter::make_diamond (param begin, param end) {
   vertex_list vl;
   vertex diam[4];
   diam[0].xpos = 0;
   diam[0].ypos = 0;
   diam[1].xpos = from_string<GLfloat>(*(--end)) * tan(M_PI/6);
   diam[1].ypos = from_string<GLfloat>(*end);
   diam[2].xpos = from_string<GLfloat>(*begin);
   diam[2].ypos = from_string<GLfloat>(*end);
   diam[3].xpos = from_string<GLfloat>(*end)*tan(M_PI/6)*2;
   diam[3].ypos = 0;
   for (size_t i = 0; i < 4; ++i)
      vl.push_back(diam[i]);
   return make_shared<diamond> (vl);
}

shape_ptr interpreter::make_triangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vl;
   vertex vtx;
   int flipflop = true;
   for (size_t i = 0; begin != end, i < 6; ++begin, ++i) {
      if(flipflop) {
	 vtx.xpos = from_string<GLfloat>(*begin);
	 flipflop = not flipflop;
      }
      else {
	 vtx.ypos = from_string<GLfloat>(*begin);
	 vl.push_back(vtx);
	 flipflop = not flipflop;
      }
   }
   return make_shared<triangle> (vl);
}

shape_ptr interpreter::make_right_triangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vl;
   vertex tri[3];
   tri[0].xpos = 0;
   tri[0].ypos = 0;
   tri[1].xpos = 0;
   tri[1].ypos = from_string<GLfloat>(*(--end));
   tri[2].xpos = from_string<GLfloat>(*begin);
   tri[2].ypos = 0;
   for (size_t i = 0; i < 3; ++i) {
      vl.push_back(tri[i]);
      //cout << tri[i].xpos << " , " << tri[i].ypos << endl;
   }
   return make_shared<right_triangle> (vl);
}

shape_ptr interpreter::make_isosceles (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vl;
   vertex tri[3];
   tri[0].xpos = 0;
   tri[0].ypos = 0;
   tri[1].xpos = from_string<GLfloat>(*begin) / 2;
   tri[1].ypos = from_string<GLfloat>(*(--end));
   tri[2].xpos = from_string<GLfloat>(*begin);
   tri[2].ypos = 0;
   for (size_t i = 0; i < 3; ++i) {
      vl.push_back(tri[i]);
   //   cout << tri[i].xpos << " , " << tri[i].ypos << endl;
   }
   return make_shared<isosceles> (vl);
}

shape_ptr interpreter::make_equilateral (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list vl;
   vertex tri[3];
   tri[0].xpos = 0;
   tri[0].ypos = 0;
   tri[1].xpos = from_string<GLfloat>(*begin) / 2;
   tri[1].ypos = from_string<GLfloat>(*begin) * sin(M_PI/3);
   tri[2].xpos = from_string<GLfloat>(*begin);
   tri[2].ypos = 0;
   for (size_t i = 0; i < 3; ++i) 
      vl.push_back(tri[i]);
   return make_shared<equilateral> (vl);
}
