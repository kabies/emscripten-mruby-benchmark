#include <mruby.h>
#include <mruby/irep.h>
#include <mruby/dump.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <mruby/data.h>
#include <stdbool.h>

typedef struct {
  int x;
  int y;
  int w;
  int h;
} Rect;

static struct mrb_data_type const mrb_rect_data_type = { "Rect", mrb_free };

static mrb_value mrb_rect_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int x,y,w,h;
  mrb_get_args(mrb, "iiii", &x, &y, &w, &h );

  Rect* r = DATA_PTR(self);
  if (r == NULL) {
    r = mrb_malloc(mrb, sizeof(Rect));
    if (NULL == r) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }

  DATA_PTR(self) = r;
  DATA_TYPE(self) = &mrb_rect_data_type;

  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;

  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@x"), mrb_fixnum_value(x) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@y"), mrb_fixnum_value(y) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@w"), mrb_fixnum_value(w) );
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb,"@h"), mrb_fixnum_value(h) );

  return self;
}


static mrb_value mrb_rect_find_via_iv(mrb_state *mrb, mrb_value self)
{
  mrb_value rects;
  mrb_int l,r,t,b; // left right top bottom

  mrb_get_args(mrb, "A", &rects );

  l = mrb_fixnum( mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@x")) );
  r = l + mrb_fixnum( mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@w")) );
  b = mrb_fixnum( mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@y")) );
  t = b + mrb_fixnum( mrb_iv_get(mrb, self, mrb_intern_cstr(mrb,"@h")) );

  for (int i = 0; i < RARRAY_LEN(rects); ++i) {
    mrb_value rect = RARRAY_PTR(rects)[i];

    mrb_int dl = mrb_fixnum( mrb_iv_get(mrb, rect, mrb_intern_cstr(mrb,"@x")) );
    mrb_int dr = dl + mrb_fixnum( mrb_iv_get(mrb, rect, mrb_intern_cstr(mrb,"@w")) );
    mrb_int db = mrb_fixnum( mrb_iv_get(mrb, rect, mrb_intern_cstr(mrb,"@y")) );
    mrb_int dt = db + mrb_fixnum( mrb_iv_get(mrb, rect, mrb_intern_cstr(mrb,"@h")) );

    if( l <= dr && dl <= r && b <= dt && db <= t ) {
      return mrb_fixnum_value(i);
    }

  }

  return mrb_nil_value();
}

static mrb_value mrb_rect_find_via_struct(mrb_state *mrb, mrb_value self)
{
  Rect* rect;
  mrb_value rects;
  int l,r,t,b; // left right top bottom

  mrb_get_args(mrb, "A", &rects );

  rect = DATA_PTR(self);
  l = rect->x;
  r = rect->x + rect->w;
  t = rect->y + rect->h;
  b = rect->y;

  for (int i = 0; i < RARRAY_LEN(rects); ++i) {
    mrb_value obj = RARRAY_PTR(rects)[i];
    rect = DATA_PTR(obj);

    int dl = rect->x;
    int dr = rect->x + rect->w;
    int dt = rect->y + rect->h;
    int db = rect->y;

    if( l <= dr && dl <= r && b <= dt && db <= t ) {
      return mrb_fixnum_value(i);
    }
  }

  return mrb_nil_value();
}

static void create_mruby_inner_methods(mrb_state* mrb)
{
  struct RClass *rect;
  rect = mrb_define_class(mrb, "Rect", mrb->object_class);
  MRB_SET_INSTANCE_TT(rect, MRB_TT_DATA);
  mrb_define_method(mrb, rect, "initialize", mrb_rect_initialize, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, rect, "find_via_iv", mrb_rect_find_via_iv, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect, "find_via_struct", mrb_rect_find_via_struct, MRB_ARGS_REQ(1));
}

int main(int argc, char* argv[])
{
  mrb_state *mrb = mrb_open();
  create_mruby_inner_methods(mrb);
  FILE *fp = fopen("assets/sample.mrb","rb");
  mrb_value obj = mrb_load_irep_file(mrb,fp);

  if (mrb->exc) {
    if (mrb_undef_p(obj)) {
      mrb_p(mrb, mrb_obj_value(mrb->exc));
    } else {
      mrb_print_error(mrb);
    }
  }

  return 0;
}
