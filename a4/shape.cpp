// $Id: shape.cpp,v 1.7 2014-05-08 18:32:56-07 - - $

#include <typeinfo>
#include <unordered_map>
#include <cmath>
using namespace std;

#include "shape.h"
#include "util.h"

vertex get_aver_vertices(const vertex_list &);
vertex_list update_center(const vertex_list &, 
                          const vertex &); 

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
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

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
   cout << "shape ctor\n"; //del
}

text::text (void* glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
   DEBUGF ('c', this);
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
   cout << "call elllips ctor\n"; //del
   cout << "parameters: " << width << ", " << height << endl; // del
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}


polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
   cout << "call polygon ctor\n";
   //auto it = vertices.begin();
   //for ( ; it != vertices.end(); ++it) 
      //cout << "{ " << it->xpos << " , " << it->ypos << " }" << endl;
}

//polygon::polygon (const GLfloat & width, const GLfloat & height) {
   //if (vertices.size() == 3) {
      //vertex tri[3];
      //tri[0].xpos = 0;
      //tri[0].ypos = 0;
      //tri[1].xpos = width;
      //tri[1].ypos = 0;
      //tri[2].xpos = width / 2;
      //tri[2].ypos = height;
      //for (size_t i = 0; i < 3; ++i)
         //vertices.push_back(tri[i]);
   //}
   //else {
      //vertex rect[4];
      //rect[0].xpos = 0;
      //rect[0].ypos = 0;
      //rect[1].xpos = 0;
      //rect[1].ypos = height;
      //rect[2].xpos = width;
      //rect[2].ypos = height;
      //rect[3].xpos = width;
      //rect[3].ypos = 0;
      //for (size_t i = 0; i < 4; ++i) 
         //vertices.push_back(rect[i]);
   //}
//}


rectangle::rectangle (const vertex_list& vertices): 
            polygon(vertices) {
   //DEBUGF ('c', this << "(" << width << "," << height << ")");
   cout << "call rectangle\n";
   //cout << "rect para: " << width << " , " << height << endl;

}

square::square (const vertex_list& vertices): rectangle (vertices) {
   DEBUGF ('c', this);
}

diamond::diamond (const vertex_list& vertices): polygon (vertices) {
}

triangle::triangle (const vertex_list& vertices): polygon (vertices) {
}

right_triangle::right_triangle (const vertex_list& vertices): 
      triangle (vertices) {}

isosceles::isosceles (const vertex_list& vertices): triangle (vertices) {
}

equilateral::equilateral (const vertex_list& vertices): 
   triangle (vertices) {}

void text::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
}

void ellipse::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin (GL_POLYGON);
   glEnable (GL_LINE_SMOOTH);
   glColor3ubv (color.ubvec);
   const float delta = 2 * M_PI / 32;
   float width = dimension.xpos;
   float height = dimension.ypos;
   for (float theta = 0; theta < 2 * M_PI; theta += delta) {
      float x = width * cos (theta) + center.xpos;
      float y = height * sin (theta) + center.ypos;
      glVertex2f (x, y);
   }
   glEnd();
   cout << "end\n";
}

void polygon::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   //cout << "av: " << aver_ver.xpos << " , " << aver_ver.ypos << endl;
   
   vertex_list vl = update_center(vertices, center);
   //auto it = vl.begin();
   //cout << "center: \n";
   //for ( ; it != vl.end(); ++it) 
      //cout << "{ " << it->xpos << " , " << it->ypos << " }" << endl;
   glBegin (GL_POLYGON);
   glColor3ubv (color.ubvec);
   for (size_t i = 0; i < vl.size(); ++i)
      glVertex2f(vl.at(i).xpos, vl.at(i).ypos);
   glEnd();
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

vertex get_aver_vertices(const vertex_list & vl) {
   vertex av;
   av.xpos = 0;
   av.ypos = 0;
   auto it = vl.begin();
   for (; it != vl.end(); ++it) {
      av.xpos += it->xpos;
      av.ypos += it->ypos;
   }
   av.xpos = av.xpos / vl.size();
   av.ypos = av.ypos / vl.size();
   return av;
}

vertex_list update_center(const vertex_list & vl, 
                          const vertex & cen) {
   vertex_list res;
   vertex av = get_aver_vertices(vl);
   vertex diff;
   diff.xpos = cen.xpos - av.xpos;
   diff.ypos = cen.ypos - av.ypos;
   //cout << "diff.xpos: " << diff.xpos 
   //<< " diff.ypos: " << diff.ypos << endl;
   for (size_t i = 0; i < vl.size(); ++i) {
      vertex v;
      v.xpos = vl.at(i).xpos + diff.xpos;
      v.ypos = vl.at(i).ypos + diff.ypos;
      res.push_back(v);
   }
   return res;
}


