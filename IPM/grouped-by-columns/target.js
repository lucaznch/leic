// Target class (position and width)
class Target
{
  constructor(x, y, w, h, l, id, prefix)
  {
    this.x      = x;
    this.y      = y;
    this.width  = w;
    this.height = h;
    this.label  = l;
    this.id     = id;
    this.prefix = prefix;
    this.suffix = this.label.substring(this.label.length - 2);
  }
    
  /*
  clicked(mouse_x, mouse_y)
  {
    return (mouse_x > this.x-(this.width/2) && mouse_x < this.x+(this.width/2)) 
      && (mouse_y > this.y-(this.height/2) && mouse_y < this.y+(this.height/2))
  }*/

  // Checks if a mouse click took place
  // within the target
  clicked(mouse_x, mouse_y)
  {
    return (mouse_x > this.x && mouse_x <= this.x + this.width) 
      && (mouse_y > this.y && mouse_y <= this.y + this.height);
  }

  // Draws the target (i.e., a circle)
  // and its label
  draw()
  {
    fill(this.getColor());
    rect(this.x, this.y, this.width, this.height);

    // Draw label
    textFont('Arial', 18);
    textStyle(BOLD); // Make the text bold
    fill(color(0,0,0));
    textAlign(CENTER, CENTER);
    text(this.label, this.x+this.width/2, this.y+this.height/2);

    // Draw prefix under the label
    textFont('Arial', 16);
    fill(this.getSuffixColor());
    text("(-" + this.suffix + ")", this.x+this.width/2, this.y+this.height/2 + 15);
  }

  getColor() {
    const colorMap = {
      'Bac': color(221,188,149),
      'Bad': color(221 - 40,188 - 40,149 - 40),
      'Baf': color(221 + 40,188 + 40,149 + 40),
      'Bag': color(221 - 60,188 - 60,149 - 60),
      
      'Bah': color(221,188,149),
      'Bak': color(221 - 40,188 - 40,149 - 40),
      'Bal': color(221 + 40,188 + 40,149 + 40),
      'Bam': color(221 - 60,188 - 60,149 - 60),

      'Ban': color(221,188,149),
      'Bao': color(221 - 40,188 - 40,149 - 40),
      'Bar': color(221 + 40,188 + 40,149 + 40),
      'Bat': color(221 - 60,188 - 60,149 - 60),

      'Bay': color(221,188,149),

      "Bei": color(251,195,176),
      "Bel": color(251 - 40,195 - 40,176 - 40),
      "Ben": color(251 + 40,195 + 40,176 + 40),
      "Bep": color(251 - 60,195 - 60,176 - 60),

      "Ber": color(251,195,176),

      "Bhi": color(245,150,107),
      "Bho": color(245 - 40,150 - 40,107 - 40),
      "Bhu": color(245 + 40,150 + 40,107 + 40),

      "Bia": color(254, 224, 139),
      "Bik": color(254 - 40, 224 - 40, 139 - 40),
      "Bil": color(254 + 40, 224 + 40, 139 + 40),
      "Bir": color(254 - 60, 224 - 60, 139 - 60),

      "Bis": color(254, 224, 139),

      "Blo": color(255, 255, 191),

      "Bne": color(206,252,248),
      

      "Boc": color(171, 221, 164),
      "Bog": color(171 - 40, 221 - 40, 164 - 40),
      "Bol": color(171 + 40, 221 + 40, 164 + 40),
      "Bos": color(171 - 60, 221 - 60, 164 - 60),

      'Bra': color(216, 224, 168),
      'Bre': color(216 - 40, 224 - 40, 168 - 40),
      'Bri': color(216 + 40, 224 + 40, 168 + 40),
      'Brn': color(216 - 60, 224 - 60, 168 - 60),

      'Bru': color(216, 224, 168),

      'Buc': color(185,226,245),
      'Bud': color(185 - 40,226 - 40,245 - 40),
      'Bue': color(185 + 40,226 + 40,245 + 40),
      'Bur': color(185 - 60,226 - 60,245 - 60),

      'Bus': color(185,226,245),
      'But': color(185 - 40,226 - 40,245 - 40),

      'Byd': color(205,198,255),

      'default': color(255, 255, 255)
    };
  
    return colorMap[this.prefix] || colorMap['default'];
  }

  getSuffixColor() {
    const colorMap = {
      'Ba': color(255 - (221 + 40) + 70,255- (188 + 40) ,255 -(149 + 40) + 70),
      'Br': color(100+255-(186 + 40),255-( 184 + 40) + 20,20 + 255- (128 + 40)),
      'Be': color(10, 100, 160),
      'Bu': color(255-185,255-226,255-245 + 100),
      'Bi': color(255-(254 + 40), 255-(224 + 40),255-( 139 + 40)),
      'Bo': color(255-(171 + 40), 255-(221 + 40),255-( 164 + 40)),
      'Bh': color(255 -(245 + 40),255-(150 + 40),255-(107 + 40)),
      'By': color(94, 79, 162),
      'Bl': color(0, 0, 255- 191),
      'Bé': color(95, 34, 12),
      'Bn': color(255-230, 255-245, 255-152),
      'default': color(255, 255, 255)
    };

    return colorMap[this.prefix.substring(0,2)];
  }
}

function getComplementaryColor(colorCode) {
  let r = 255 - red(colorCode);
  let g = 255 - green(colorCode);
  let b = 255 - blue(colorCode);
  return color(r, g, b);
}