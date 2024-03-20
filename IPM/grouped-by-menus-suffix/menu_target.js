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
        fill(color(0,0,0));
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
      fill(color(0,0,0));
      textAlign(CENTER, CENTER);
      textStyle(BOLD)
      textFont("Arial", 40);
      text(this.label, this.x + this.width / 2, this.y + (this.height / 2) + getFontHeight("Arial", 40, BOLD) / 8)
      textStyle(NORMAL);
    }
  }
  
  select() {
    if (this.selected_options[this.label] && this.selected_options["PELO MENOS UM"]) {
      this.selected_options[this.label] = true;
      this.selected_options["PELO MENOS UM"] = true;
    } else {
      for (var key in this.selected_options) this.selected_options[key] = false
      this.selected_options[this.label] = true;
      this.selected_options["PELO MENOS UM"] = true;
    }
  }


getColor()
{
  const colorMap = {
    "a": color(88, 163, 153),    // Vermelho
    "d": color(168, 205, 159),   // Laranja
    "e": color(88, 163, 153),  // Laranja-claro
    "g": color(168, 205, 159),  // Amarelo
    "h": color(88, 163, 153),  // Verde-limão
    "i": color(168, 205, 159),  // Verde-claro
    "k": color(88, 163, 153),    // Verde
    "l": color(168, 205, 159),   // Verde-água
    "m": color(88, 163, 153),  // Verde-azulado
    "n": color(168, 205, 159),  // Ciano
    "o": color(88, 163, 153),  // Azul-claro
    "r":color(168, 205, 159),   // Azul
    "s": color(88, 163, 153),    // Azul-escuro
    "t": color(168, 205, 159),   // Roxo-azulado
    "u": color(88, 163, 153),  // Roxo
    "v": color(168, 205, 159),  // Rosa
    "y": color(88, 163, 153),  // Rosa-claro
    "z":color(168, 205, 159),   // Rosa-escuro
    "default": color(128, 128, 128) // Cinza (para qualquer outra letra não especificada)
  };

  return colorMap[this.label] || colorMap['default'];
}




  
  getDarkerColor() {
    const colorMap = {
      "a": color(255, 0, 0),    // Vermelho
      "d": color(255, 85, 0),   // Laranja
      "e": color(255, 170, 0),  // Laranja-claro
      "g": color(255, 255, 0),  // Amarelo
      "h": color(170, 255, 0),  // Verde-limão
      "i": color(85, 255, 0),   // Verde-claro
      "k": color(0, 255, 0),    // Verde
      "l": color(0, 255, 85),   // Verde-água
      "m": color(0, 255, 170),  // Verde-azulado
      "n": color(0, 255, 255),  // Ciano
      "o": color(0, 170, 255),  // Azul-claro
      "r": color(0, 85, 255),   // Azul
      "s": color(0, 0, 255),    // Azul-escuro
      "t": color(85, 0, 255),   // Roxo-azulado
      "u": color(170, 0, 255),  // Roxo
      "v": color(255, 0, 255),  // Rosa
      "y": color(255, 0, 170),  // Rosa-claro
      "z": color(255, 0, 85),   // Rosa-escuro
      "default": color(128, 128, 128) // Cinza (para qualquer outra letra não especificada)
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