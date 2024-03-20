// Target class (position and width)
class Target
{
  constructor(x, y, w, h, l, id)
  {
    this.x      = x;
    this.y      = y;
    this.width  = w;
    this.height = h;
    this.label  = l;
    this.id     = id;
    this.prefix = removeAccents(l.substring(0,5))
    this.selected = false;
  }
  
  // Checks if a mouse click took place
  // within the target
  clicked(mouse_x, mouse_y)
  {
    return (mouse_x > this.x && 
            mouse_x < this.x + this.width && 
            mouse_y > this.y && 
            mouse_y < this.y + this.height);
  }
  
  // Draws the target (i.e., a circle)
  // and its label
  draw() {
    // Draw target
    fill(this.getColor());                 
    rect(this.x, this.y, this.width, this.height);
    
    let space_between = (this.height - (getFontHeight("Arial", 13) + getFontHeight("Arial", 20))) / 2
    
    // Draw label
    textFont("Arial", 16);
    fill(this.getDarkerColor());
    textAlign(CENTER, CENTER);
    textStyle(BOLD)
    text(this.label, this.x + this.width / 2, this.y + this.height - space_between - getFontHeight("Arial", 13)/2);

    // Draw "ABC" inside the rectangle
    fill(this.getLighterColor()); // Escolha a cor que desejar para o texto
    textAlign(CENTER);
    textSize(20); // Escolha o tamanho do texto
    textStyle(BOLD)
    text(this.prefix.toUpperCase(), this.x + this.width / 2, this.y + space_between + getFontHeight("Arial", 20)/2); // Posicione o texto acima do retângulo
  
    this.drawTitle();
  
  }
  
  getColor()
  {
    const colorMap = {
      'Bac': color(128, 15, 60), //1
      'Bad': color(160, 19, 75), //1
      'Baf': color(128, 15, 60), //1
      'Bag': color(160, 19, 75),         //4
      'Bah': color(128, 15, 60), //2
      'Bak': color(160, 19, 75), //1
      'Bal': color(128, 15, 60), //1
      'Bam': color(160, 19, 75), // 1
      'Ban': color(128, 15, 60), // 2
      
      'Bao': color(160, 19, 75), // 2
      'Bar': color(128, 15, 60),       // 7x
      'Bat': color(160, 19, 75), // 2
      'Bay': color(128, 15, 60), // 2

      
      'Bra': color(189, 46, 62),
      'Bre': color(215, 91, 105), //.
      'Bri': color(189, 46, 62),
      'Brn': color(215, 91, 105),
      'Bru': color(189, 46, 62),

      'Bec': color(215, 99, 60), //.
      'Bei': color(171, 71, 38),
      'Bel': color(215, 99, 60),
      'Ben': color(171, 71, 38),
      'Bep': color(215, 99, 60),
      'Ber': color(171, 71, 38),

      'Buc': color(152, 105, 60), // 
      'Bud': color(109, 75, 44),
      'Bue': color(152, 105, 60),
      'Bur': color(109, 75, 44),
      'Bus': color(152, 105, 60),
      'But': color(109, 75, 44),

      'Bia': color(182, 155, 9), //.
      'Bik': color(146, 125, 7),
      'Bil': color(182, 155, 9),
      'Bir': color(146, 125, 7),
      'Bis': color(182, 155, 9),

      'Boc': color(122, 173, 115), //.
      'Bog': color(92, 145, 85),
      'Bol': color(122, 173, 115),
      'Bos': color(92, 145, 85),

      'Bhi': color(19, 103, 40), //.
      'Bho': color(20, 130, 47),
      'Bhu': color(19, 103, 40),

      'Byd': color(18, 71, 11),

      'Blo': color(102, 204, 165),

      'Bne': color(189, 136, 189),
      'default': color(133, 133, 133)
    };
    if (this.selected) {
      return colorMap['default']
    }
  
    return colorMap[this.prefix.substring(0,3)] || colorMap['default'];
  }
  
  getLighterColor() {
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
      'Bh': color(20, 130, 47),
      'By': color(18, 71, 11),
      'Bl': color(102, 204, 165),
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


  drawTitle() {
    if (this.label == "Bacolod") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BA", this.x - getTextWidth("BA", "Arial", 25)/2, this.y + this.height/2);  
    }
    else if (this.label == "Baoding") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BA", this.x - getTextWidth("BA", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Béchar") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BE", this.x - getTextWidth("BE", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Bhilai") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(30);
      textStyle(BOLD);
      text("BH", this.x - getTextWidth("BH", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Bial Podlaska") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BI", this.x - getTextWidth("BI", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Bloemfontein") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BL", this.x - getTextWidth("BL", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Bne Brak") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BN", this.x - getTextWidth("BN", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Bochum") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BO", this.x - getTextWidth("BO", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Bradford") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BR", this.x - getTextWidth("BR", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Bucaramanga") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BU", this.x - getTextWidth("BU", "Arial", 25)/2, this.y + this.height/2); 
    }
    else if (this.label == "Bydgoszcz") {
      fill(this.getColor());
      textAlign(CENTER);
      textSize(25);
      textStyle(BOLD);
      text("BY", this.x - getTextWidth("BY", "Arial", 25)/2, this.y + this.height/2); 
    }
    
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


