// Target class (position and width)
class Menu_Target
{
  constructor(x, y, w, h, l, selected_options)
  {
    this.x      = x;
    this.y      = y;
    this.width  = w;
    this.height = h;
    this.label  = l;
    this.selected_options = selected_options;
  }
  
  clicked(mouse_x, mouse_y)
  {
    if ((mouse_x > this.x && mouse_x < this.x + this.width && mouse_y > this.y && mouse_y < this.y + this.height)) {
      return true;
    } else if ((mouse_x > this.x && mouse_x < this.x + this.width && mouse_y > this.y && mouse_y < this.y + this.height) && this.selected_options[this.label]) {
      return true;        
    }
    return false;
  }
  
  draw() {
    if (this.selected_options["PELO MENOS UM"]) {
      
      /*if (this.selected_options[this.label]) {*/
      
        // Draw target
        fill(this.getColor());                 
        rect(this.x, this.y, this.width, this.height);

        // Draw label
        fill(this.getDarkerColor());
        textAlign(CENTER, CENTER);
        textStyle(BOLD)
        textFont("Arial", 40);
        text(this.label, this.x + this.width / 2, this.y + (this.height / 2) + getFontHeight("Arial", 40, BOLD) / 8)
        textStyle(NORMAL);
      
      /*
      } 
      else {
        // do nothing ehehehe
      } */

    } else {
      // Draw target
      fill(this.getColor());                 
      rect(this.x, this.y, this.width, this.height);

      // Draw label
      fill(this.getDarkerColor());
      textAlign(CENTER, CENTER);
      textStyle(BOLD)
      textFont("Arial", 40);
      text(this.label, this.x + this.width / 2, this.y + (this.height / 2) + getFontHeight("Arial", 40, BOLD) / 8)
      textStyle(NORMAL);
    }
  }
  
  select() {
    if (this.selected_options[this.label] && this.selected_options["PELO MENOS UM"]) {
      this.selected_options[this.label] = false;
      this.selected_options["PELO MENOS UM"] = false;
    } else {
      for (var key in this.selected_options) this.selected_options[key] = false
      this.selected_options[this.label] = true;
      this.selected_options["PELO MENOS UM"] = true;
    }
  }

  getColor()
  {
    const colorMap = {
      'Ba': color(158, 1, 66),
      'Br': color(213, 62, 79),
      'Be': color(244, 109, 67),
      'Bu': color(152, 105, 60),
      'Bi': color(182, 155, 9),
      'Bo': color(122, 173, 115),
      'Bh': color(20, 130, 47),
      'By': color(18, 71, 11),
      'Bl': color(102, 204, 165),
      'Bé': color(50, 136, 189),
      'Bn': color(189, 136, 189),
      'default': color(133, 133, 133)
    };
  
    return colorMap[this.label] || colorMap['default'];
  }
  
  getDarkerColor() {
    const colorMap = {
      'Ba': color(158, 1, 66),
      'Br': color(213, 62, 79),
      'Be': color(244, 109, 67),
      'Bu': color(152, 105, 60),
      'Bi': color(182, 155, 9),
      'Bo': color(122, 173, 115),
      'Bh': color(20, 130, 47),
      'By': color(18, 71, 11),
      'Bl': color(102, 204, 165),
      'Bé': color(50, 136, 189),
      'Bn': color(189, 136, 189),
      'default': color(133, 133, 133)
    };

    let baseColor = colorMap[this.label] || colorMap['default'];

    // Define a value for how much darker you want the color to be
    let darknessFactor = 0.4; // Change this factor as per your requirement

    // Calculate the darker color
    let darkerColor = color(
      red(baseColor) * darknessFactor,
      green(baseColor) * darknessFactor,
      blue(baseColor) * darknessFactor
    );

    return darkerColor;
  }
}

function getFontHeight(fontType, fontSize, fontStyle) {
  textFont(fontType);
  textSize(fontSize);
  if (fontStyle) {
    textStyle(fontStyle);
  }
  return textAscent()
}
