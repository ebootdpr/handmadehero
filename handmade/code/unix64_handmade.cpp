#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <unistd.h>

// #include <unistd.h>

// headers:::
struct Historia;
void _AllocateNewHistoryChunk(Historia *h);

struct dumbTimeLL {
  long seco;
  long nano;
};

struct Entity {
  unsigned int id;
  int x;
  int y;
  unsigned int flags;
  Entity() {
    static int currentId = 0;
    id = currentId++;
    x = 0;
    y = 0;
    flags = 1;
  }
  Entity(int x0, int y0, int flags0) {
    static int currentId = 0;
    id = currentId++;
    x = x0;
    y = y0;
    flags = flags0;
  }
};

struct isRectangle{
  int len_points;
  int points[10];
  isRectangle(unsigned short width,unsigned short heigth){
    len_points=10;
    points[0] = 0;
    points[1] = 0;

    points[2] = width;
    points[3] = 0;

    points[4] = width;
    points[5] = heigth;

    points[6] = 0;
    points[7] = heigth;

    points[8] = 0;
    points[9] = 0;
  }
  isRectangle(){
    len_points=10;
    points[0] = 0;
    points[1] = 0;

    points[2] = 50;
    points[3] = 0;

    points[4] = 50;
    points[5] = 80;

    points[6] = 0;
    points[7] = 80;

    points[8] = 0;
    points[9] = 0;
  }
};

struct ObeysPhysics {
  int velx;
  int vely;
  int accx;
  int accy;
  int mass;
};

struct Character : Entity,ObeysPhysics,isRectangle
{
  char name[16];
};

const unsigned int MAX_HISTORY = 400;

struct HistoryNode {
  unsigned int len;
  int pos[MAX_HISTORY];
  HistoryNode *prev;
};

// Historia is a stack Data Structure that hold history items.
struct Historia {
  Historia() { _AllocateNewHistoryChunk(this); }
  unsigned int len;
  HistoryNode *latest_history_chunk;
};

// AllocateNewHistoryChunk creates a heap allocated HistoryNode
// Sets the .prev of that node to the latest of Historia
// Sets the latest node pointer to the recent created one
// Adds 1 to the length of Historia
void _AllocateNewHistoryChunk(Historia *h) {
  HistoryNode *heap_chunk = (HistoryNode *)malloc(sizeof(HistoryNode));
  heap_chunk->prev = h->latest_history_chunk;
  heap_chunk->len = 0;
  h->latest_history_chunk = heap_chunk;
  h->len = h->len + 1;
}

// DeallocateLatestHistoryChunk
// Redirect the latest pointer node of Historia to the .prev.
// Frees the latest pointer
// Reduces the length of Historia by 1
void _DeallocateLatestHistoryChunk(Historia *h) {
  if (h->len == 1)
    return; // otherwise next line segfaults on first flush
  HistoryNode *ptr_to_free = h->latest_history_chunk;
  h->latest_history_chunk = ptr_to_free->prev;
  h->len = h->len - 1;
  free(ptr_to_free);
}

void addToHistory(Historia *h, int x0, int y0, int x1, int y1) {
  HistoryNode *history = h->latest_history_chunk;
  if (h->len != 0 && history->len < MAX_HISTORY) {
    history->pos[history->len + 1] = x0;
    history->pos[history->len + 2] = y0;
    history->pos[history->len + 3] = x1;
    history->pos[history->len + 4] = y1;
    history->len += 4;
  } else {
    _AllocateNewHistoryChunk(h);
    addToHistory(h, x0, y0, x1, y1);
  }
}

int main(int argc, char *argv[]) {
  Character ch{};
  
  Display *display;
  Window window;
  XEvent event;
  int screen;
  int last_x = -1;
  int last_y = -1;
  /* Open connection to X server */
  display = XOpenDisplay(NULL);
  if (display == NULL) {

    return 1;
  }

  screen = DefaultScreen(display);

  /* Create window */
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 400,
                               400, 0, BlackPixel(display, screen),
                               WhitePixel(display, screen));

  /* Select event types */
  // XSelectInput(display, window, ExposureMask | KeyPressMask |
  // ButtonPressMask);
  XSelectInput(display, window,
               ExposureMask | KeyPressMask | ButtonPressMask |
                   PointerMotionMask);

  /* Map window on the screen */
  XMapWindow(display, window);

  /* Event loop */
  bool isDrawing = false;
  Historia a;
  
  while (1) {
    XNextEvent(display, &event);
    if (event.type == ResizeRequest) {
      /*  XDrawLine(display, window, DefaultGC(display, screen), 22,
                 44, 222, 444); */
    }
    
    // XDrawPoint(display, window, DefaultGC(display, screen),event.xmotion.x,
    // event.xmotion.y);
    if (event.type == ButtonRelease && event.xbutton.button == Button1) {
      isDrawing = false;
    }
    if (event.type == ButtonPress && event.xbutton.button == Button1) {
      isDrawing = !isDrawing;
      last_x = event.xmotion.x;
      last_y = event.xmotion.y;
    }
    if (event.type == Expose) {
      for (unsigned int i = 0; i < ch.len_points; i += 2) {
        // When i == 2,4,5,7 then ch.points[i]!= 0
          XDrawLine(display, window, DefaultGC(display, screen), ch.x+ch.points[i],
                    ch.y+ch.points[i+1], ch.x+ch.points[i+2],ch.y+ch.points[i+3]);
        }
      // XFlush(display);
    }

    KeySym key = XLookupKeysym(&event.xkey, 0);
    KeyCode keycode = XKeysymToKeycode(display, key);
    
    if (keycode == XKeysymToKeycode(display, 'x')) {
      /* 'q' key is pressed, exit the program */
      // XFlush(display);
      XClearWindow(display, window);
      ch.x++;
      for (unsigned int i = 0; i < ch.len_points; i += 2) {
        // When i == 2,4,5,7 then ch.points[i]!= 0
          XDrawLine(display, window, DefaultGC(display, screen), ch.x+ch.points[i],
                    ch.y+ch.points[i+1], ch.x+ch.points[i+2],ch.y+ch.points[i+3]);
        }
    }

    if (keycode == XKeysymToKeycode(display, 'q')) {
      /* 'q' key is pressed, exit the program */
      goto last_line_of_main;
    }
    if (keycode == XKeysymToKeycode(display, 'z')) {
      /* 'q' key is pressed, exit the program */
      if (a.len > 0 || a.latest_history_chunk != NULL) {

        HistoryNode b = *a.latest_history_chunk;
        for (unsigned int i = 1; i < b.len; i += 4) {
          XDrawLine(display, window, DefaultGC(display, screen), b.pos[i],
                    b.pos[i + 1], b.pos[i + 2], b.pos[i + 3]);
        }
        _DeallocateLatestHistoryChunk(&a);
      }
    }
    if (event.type == MotionNotify && isDrawing) {

      /* Draw line from last clicked point to current point */
      if (last_x != event.xmotion.x && last_y != event.xmotion.y) {
        XDrawLine(display, window, DefaultGC(display, screen), last_x, last_y,
                  event.xmotion.x, event.xmotion.y);
        addToHistory(&a, last_x, last_y, event.xmotion.x, event.xmotion.y);

        /* Draw pixel at current point */
        /* XDrawPoint(display, window, DefaultGC(display, screen),
                   event.xmotion.x, event.xmotion.y); */
        /* Update last clicked point */
        last_x = event.xmotion.x;
        last_y = event.xmotion.y;
      }

      /* Flush output to the server */
    }
  }

  /* Clean up */
  XDestroyWindow(display, window);
  XCloseDisplay(display);
last_line_of_main:
  return 0;
}
