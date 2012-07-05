function test1

  # Features tested:
  # - menu creation
  # - context menus
  # - object picking
  # - property listener

  f = figure;

  x = 0:0.1:2*pi;
  y1 = sin (x);
  y2 = cos (x);

  h = plot (x, y1, x, y2);
  set (h(1), "marker", "o", "markersize", 15);

  ma = uicontextmenu;
  uimenu (ma, "Label", "Hello from axes object");
  set (gca, "uicontextmenu", ma);

  mf = uicontextmenu;
  uimenu (mf, "Label", "Hello from figure object");
  set (gcf, "uicontextmenu", mf);

  ml = uicontextmenu;
  ml_style = uimenu (ml, "Label", "Change Line Style");
  uimenu (ml_style, "Label", "Solid", "Callback", {@set_line_style, "-"});
  uimenu (ml_style, "Label", "Dashed", "Callback", {@set_line_style, "--"});
  uimenu (ml_style, "Label", "Dotted", "Callback", {@set_line_style, ":"});
  ml_color = uimenu (ml, "Label", "Change Color");
  uimenu (ml_color, "Label", "Red", "Callback", {@set_color, "r"});
  uimenu (ml_color, "Label", "Blue", "Callback", {@set_color, "b"});
  uimenu (ml_color, "Label", "Green", "Callback", {@set_color, [0, 0.6, 0]});
  uimenu (ml_color, "Label", "Yellow", "Callback", {@set_color, [0.6, 0.6, 0]});
  uimenu (ml_color, "Label", "Magenta", "Callback", {@set_color, "m"});
  ml_width = uimenu (ml, "Label", "Change Line Width");
  uimenu (ml_width, "Label", "0.5pt", "Callback", {@set_line_width, 0.5});
  uimenu (ml_width, "Label", "1pt", "Callback", {@set_line_width, 1});
  uimenu (ml_width, "Label", "2pt", "Callback", {@set_line_width, 2});
  uimenu (ml_width, "Label", "3pt", "Callback", {@set_line_width, 3});
  set (h, "uicontextmenu", ml);

  ht = text (4, 0, "This is some text", "FontSize", 14);

  mt = uicontextmenu;
  uimenu (mt, "Label", "Hello from text object");
  mt_size = uimenu (mt, "Label", "Change Font Size");
  uimenu (mt_size, "Label", "10pt", "Callback", @(h,e) set (ht, "FontSize", 10));
  uimenu (mt_size, "Label", "12pt", "Callback", @(h,e) set (ht, "FontSize", 12));
  uimenu (mt_size, "Label", "14pt", "Callback", @(h,e) set (ht, "FontSize", 14));
  uimenu (mt_size, "Label", "16pt", "Callback", @(h,e) set (ht, "FontSize", 16));
  uimenu (mt_size, "Label", "18pt", "Callback", @(h,e) set (ht, "FontSize", 18));
  uimenu (mt_size, "Label", "20pt", "Callback", @(h,e) set (ht, "FontSize", 20));
  set (ht, "uicontextmenu", mt);

  title ("QtHandles: right-click on various objects to make context menus appear.");

  lt = text (0.2, -0.5, "Current object:");
  addlistener (gcf, "currentobject", {@gco_changed, lt});

  text (3, 0.8, "I'm not clickable", "HitTest", "off");
  text (3, 0.5, "I'm invisible", "HandleVisibility", "off");

endfunction

function set_line_style (h, e, st)
  set (gco, "linestyle", st);
endfunction

function set_color (h, e, st)
  set (gco, "color", st);
endfunction

function set_line_width (h, e, st)
  set (gco, "linewidth", st);
endfunction

function gco_changed (h, e, lt)
  oh = get (h, "currentobject");
  if (isempty (oh))
    set (lt, "string", "Current object:");
  else
    set (lt, "string", ["Current object: ", get(oh, "type"), " -> ", num2str(oh)]);
  endif
endfunction
