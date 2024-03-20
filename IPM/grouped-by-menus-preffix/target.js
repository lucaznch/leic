// Target class (position and width)
class Target
{
  constructor(x, y, w, h, l, id, selected_options)
  {
    this.x      = x;
    this.y      = y;
    this.width  = w;
    this.height = h;
    this.label  = l;
    this.id     = id;
    this.prefix = removeAccents(l.substring(0,5))
    this.selected_options = selected_options;
    this.was_selected = false;
  }
  
  // Checks if a mouse click took place
  // within the target
  clicked(mouse_x, mouse_y)
  {
    if ((mouse_x > this.x && mouse_x < this.x + this.width && mouse_y > this.y && mouse_y < this.y + this.height) && this.selected_options[this.prefix.substring(0,2)]) {
      return true;
    }
    return false
  }
  
  // Draws the target (i.e., a circle)
  // and its label
  draw() {
    if (this.selected_options["PELO MENOS UM"]) {
      if (this.selected_options[this.prefix.substring(0,2)]) {
        // Draw target
        fill(this.getColor());                 
        rect(this.x, this.y, this.width, this.height);
        
        let space_between = (this.height - (getFontHeight("Arial", 18) + getFontHeight("Arial", 24))) / 2
        
        // Draw label
        textFont("Arial", 18);
        fill(color(255,255,255));
        textAlign(CENTER, CENTER);
        textStyle(NORMAL)
        text(this.label, this.x + this.width / 2, this.y + this.height - space_between - getFontHeight("Arial", 18)/2);

        // Draw "ABC" inside the rectangle
        fill(this.getDarkerColor()); // Escolha a cor que desejar para o texto
        textAlign(CENTER);
        textSize(24); // Escolha o tamanho do texto
        textStyle(BOLD)
        text(this.label.substring(0,5).toUpperCase(), this.x + this.width / 2, this.y + this.height/2 - getFontHeight("Arial", 24)/2); // Posicione o texto acima do retângulo

        textStyle(NORMAL)
      } else {
        // do nothing ehehehe
      }
    }
  }
  
  unselect() {
    this.selected_options[this.prefix.substring(0,2)] = true;
    this.selected_options["PELO MENOS UM"] = true;
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
      'Bh': color(18, 130, 47),
      'By': color(18, 71, 11),
      'Bl': color(102, 184, 165),
      'Bé': color(50, 136, 189),
      'Bn': color(189, 136, 189),
      'default': color(133, 133, 133)
    };
    if (this.was_selected) {
      return colorMap['default']
    }
  
    return colorMap[this.prefix.substring(0,2)] || colorMap['default'];
  }
  
  getLighterColor() {
    const colorMap = {
      'Ba': color(158, 1, 66),
      'Br': color(213, 62, 79),
      'Be': color(244, 109, 67),
      'Bu': color(152, 105, 60),
      'Bi': color(182, 155, 9),
      'Bo': color(122, 173, 115),
      'Bh': color(18, 130, 47),
      'By': color(18, 71, 11),
      'Bl': color(102, 184, 165),
      'Bé': color(50, 136, 189),
      'Bn': color(189, 136, 189),
      'default': color(133, 133, 133)
    };

    let baseColor = colorMap[this.prefix.substring(0, 2)] || colorMap['default'];

    // Define a value for how much lighter you want the color to be
    let lightnessFactor = 0.6; // Change this factor as per your requirement

    // Calculate the lighter color
    let lighterColor = lerpColor(baseColor, color(255), lightnessFactor);

    return lighterColor;
  }
  
  getDarkerColor() {
    const colorMap = {
      'Ba': color(158, 1, 66),
      'Br': color(213, 62, 79),
      'Be': color(244, 109, 67),
      'Bu': color(152, 105, 60),
      'Bi': color(182, 155, 9),
      'Bo': color(122, 173, 115),
      'Bh': color(18, 130, 47),
      'By': color(18, 71, 11),
      'Bl': color(102, 184, 165),
      'Bé': color(50, 136, 189),
      'Bn': color(189, 136, 189),
      'default': color(133, 133, 133)
    };

    let baseColor = colorMap[this.prefix.substring(0, 2)] || colorMap['default'];

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
  
function removeAccents(str) {
  return str.normalize('NFD').replace(/[\u0300-\u036f]/g, '');
}

function getFontHeight(fontType, fontSize) {
  let tempDiv = createDiv();
  tempDiv.style('font-family', fontType);
  tempDiv.style('font-size', fontSize + 'px');
  tempDiv.html('Hg'); // Usando uma letra maiúscula para obter a altura máxima
  tempDiv.position(-1000, -1000); // Posiciona fora da tela
  let height = tempDiv.size().height;
  tempDiv.remove();
  return height;
}

function getTextWidth(text, fontType, fontSize) {
  let tempDiv = createDiv();
  tempDiv.style('font-family', fontType);
  tempDiv.style('font-size', fontSize + 'px');
  tempDiv.html('text');
  tempDiv.position(-1000, -1000); // Position off-screen
  let width = tempDiv.size().width;
  tempDiv.remove();
  return width;
}


