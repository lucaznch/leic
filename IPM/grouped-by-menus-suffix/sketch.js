// Bake-off #2 -- Seleção em Interfaces Densas
// IPM 2023-24, Período 3
// Entrega: até às 23h59, dois dias úteis antes do sexto lab (via Fenix)
// Bake-off: durante os laboratórios da semana de 18 de Março

// p5.js reference: https://p5js.org/reference/

// Database (CHANGE THESE!)
const GROUP_NUMBER        = 3;      // Add your group number here as an integer (e.g., 2, 3)
const RECORD_TO_FIREBASE  = true;  // Set to 'true' to record user results to Firebase

// Pixel density and setup variables (DO NOT CHANGE!)
let PPI, PPCM;
const NUM_OF_TRIALS       = 12;     // The numbers of trials (i.e., target selections) to be completed
let continue_button;
let legendas;                       // The item list from the "legendas" CSV

// Metrics (DO NOT CHANGE!)
let testStartTime, testEndTime;     // time between the start and end of one attempt (8 trials)
let hits 			      = 0;      // number of successful selections
let misses 			      = 0;      // number of missed selections (used to calculate accuracy)
let database;                       // Firebase DB  

// Study control parameters (DO NOT CHANGE!)
let draw_targets          = false;  // used to control what to show in draw()
let trials;                         // contains the order of targets that activate in the test
let current_trial         = 0;      // the current trial number (indexes into trials array above)
let attempt               = 0;      // users complete each test twice to account for practice (attemps 0 and 1)

// Target list and layout variables
var targets               = [];
var menu_targets = [];

let drawn_recently_mutex = false

var selected_options = {
  "a": false,
  "g": false,
  "n": false,
  "¡": false,
  "e": false,
  "s": false,
  "t": false,
  "m": false,
  "u": false,
  "l": false,
  "o": false,
  "z": false,
  "i": false,
  "d": false,
  "k": false,
  "r": false,
  "y": false,
  "v": false,
  "h": false,
  "PELO MENOS UM": false,
}

const GRID_ROWS           = 11;      // We divide our 80 targets in a 8x10 grid
const GRID_COLUMNS        = 14;     // We divide our 80 targets in a 8x10 grid

let ignored_size = Array.from({length: 8}, (_, i) => i + 1);

let boneco1
let boneco2

// Ensures important data is loaded before the program starts
function preload()
{
  // id,name,...
  legendas = loadTable('legendas.csv', 'csv', 'header');
  
  boneco1 = loadImage("dora.png")
  boneco2 = loadImage("kanna.png")
}

//let video;
let vid_button;

function startWarning() {
    boneco1.resize(140,190)
    image(boneco1, 20, 260);
    boneco2.resize(200, 190)
    image(boneco2, width - 190, 260);
  
    textFont("Arial", 26);
    textStyle(BOLD);
    fill(color(255, 255, 255));
    textAlign(CENTER);
    fill(color(102, 153, 255))
    text("Vê o vídeo tutorial antes de começares!", width / 2, 177);
    
    textStyle(NORMAL);
    textFont("Arial", 18);
    fill(color(255, 255, 255));
    text("Já viste o vídeo? Se sim, então já sabes como funciona tudo e estás preparado", 350, 200);
  
    
    vid_button = createButton("Clica aqui para veres o <br> vídeo TUTORIAL");
    vid_button.position(width/2 - 170, 300);
    vid_button.style('color:rgb(255,0,0)');
    vid_button.style('font-size: 30px; font-family: Arial;font-weight: bold');
    vid_button.mouseClicked(openInNewTab); 
    
    fill(color(179, 204, 255))
  textFont("Arial", 14);
  textStyle(BOLD)
    text("O vídeo tutorial tem som, mas também é legendado para caso não tenhas fones!", 350, 465)
    fill(color(0,0,0))
  
  fill(color(102, 153, 255))
  textFont("Arial", 20);
  textStyle(NORMAL)
    text("Por favor :)", width/2, 400)
    fill(color(0,0,0))

  fill(color(179, 204, 255))
      textStyle(BOLD)
      textFont("Arial", 14);
      text("nota: podes acelerar o vídeo nos 3 pontinhos", width/2, 485)
}


// Runs once at the start
function setup()
{
  createCanvas(700, 500);    // window size in px before we go into fullScreen()
  frameRate(60);             // frame rate (DO NOT CHANGE!)
  
  randomizeTrials();         // randomize the trial order at the start of execution
  drawUserIDScreen();        // draws the user start-up screen (student ID and display size)
  startWarning();
}


// Runs every frame and redraws the screen
function draw()
{
  if (draw_targets && attempt < 2)
  {
    // The user is interacting with the 6x3 target grid
    background(color(0,0,0));        // sets background to black
    
    // Print trial count at the top left-corner of the canvas
    textFont("Arial", 16);
    fill(color(255,255,255));
    textAlign(LEFT);
    text("Trial " + (current_trial + 1) + " of " + trials.length, 50, 20);
    
    drawn_recently_mutex = false
    
    // Draw all menu targets
    for (var i = 0; i < 18; i++) menu_targets[i].draw();

    // Draw all targets
	for (var i = 0; i < legendas.getRowCount(); i++) targets[i].draw();
    
    drawn_recently_mutex = true
    
    // Draws the target label to be selected in the current trial. We include 
    // a black rectangle behind the trial label for optimal contrast in case 
    // you change the background colour of the sketch (DO NOT CHANGE THESE!)
    fill(color(0,0,0));
    rect(0, height - 40, width, 40);
 
    textFont("Arial", 20); 
    fill(color(255,255,255)); 
    textAlign(CENTER); 
    text(legendas.getString(trials[current_trial],1), width/2, height - 20);
  }
}

// Print and save results at the end of 54 trials
function printAndSavePerformance()
{
  // DO NOT CHANGE THESE! 
  let accuracy			= parseFloat(hits * 100) / parseFloat(hits + misses);
  let test_time         = (testEndTime - testStartTime) / 1000;
  let time_per_target   = nf((test_time) / parseFloat(hits + misses), 0, 3);
  let penalty           = constrain((((parseFloat(95) - (parseFloat(hits * 100) / parseFloat(hits + misses))) * 0.2)), 0, 100);
  let target_w_penalty	= nf(((test_time) / parseFloat(hits + misses) + penalty), 0, 3);
  let timestamp         = day() + "/" + month() + "/" + year() + "  " + hour() + ":" + minute() + ":" + second();
  
  textFont("Arial", 18);
  background(color(0,0,0));   // clears screen
  fill(color(255,255,255));   // set text fill color to white
  textAlign(LEFT);
  text(timestamp, 10, 20);    // display time on screen (top-left corner)
  
  textAlign(CENTER);
  text("Attempt " + (attempt + 1) + " out of 2 completed!", width/2, 60); 
  text("Hits: " + hits, width/2, 100);
  text("Misses: " + misses, width/2, 120);
  text("Accuracy: " + accuracy + "%", width/2, 140);
  text("Total time taken: " + test_time + "s", width/2, 160);
  text("Average time per target: " + time_per_target + "s", width/2, 180);
  text("Average time for each target (+ penalty): " + target_w_penalty + "s", width/2, 220);

  // Saves results (DO NOT CHANGE!)
  let attempt_data = 
  {
        project_from:       GROUP_NUMBER,
        assessed_by:        student_ID,
        test_completed_by:  timestamp,
        attempt:            attempt,
        hits:               hits,
        misses:             misses,
        accuracy:           accuracy,
        attempt_duration:   test_time,
        time_per_target:    time_per_target,
        target_w_penalty:   target_w_penalty,
  }
  
  // Sends data to DB (DO NOT CHANGE!)
  if (RECORD_TO_FIREBASE)
  {
    // Access the Firebase DB
    if (attempt === 0)
    {
      firebase.initializeApp(firebaseConfig);
      database = firebase.database();
    }
    
    // Adds user performance results
    let db_ref = database.ref('G' + GROUP_NUMBER);
    db_ref.push(attempt_data);
  }
}


// Mouse button was pressed - lets test to see if hit was in the correct target
function mousePressed() 
{
  // Only look for mouse releases during the actual test
  // (i.e., during target selections)
  if (draw_targets)
  {
    for (var i = 0; i < legendas.getRowCount(); i++)
    {
      // Check if the user clicked over one of the targets
      if (targets[i].clicked(mouseX, mouseY)) 
      {
        // Checks if it was the correct target
        if (targets[i].id === trials[current_trial]+1) { hits++; }
        else misses++;
        
        targets[i].was_selected = true;

        current_trial++;              // Move on to the next trial/target
        drawn_recently_mutex = false
        targets[i].unselect()
        break;
      }
    }
    
    for (var j = 0; j < 18; j++) {
        if (menu_targets[j].clicked(mouseX, mouseY) && drawn_recently_mutex) {
          menu_targets[j].select()
        }
      }
    
    // Check if the user has completed all trials
    if (current_trial === NUM_OF_TRIALS)
    {
      testEndTime = millis();
      draw_targets = false;          // Stop showing targets and the user performance results
      printAndSavePerformance();     // Print the user's results on-screen and send these to the DB
      attempt++;                      
      
      // If there's an attempt to go create a button to start this
      if (attempt < 2)
      { // just to reset the submenu that was open because another attempt started
        selected_options = {
          "a": false,
          "g": false,
          "n": false,
          "¡": false,
          "e": false,
          "s": false,
          "t": false,
          "m": false,
          "u": false,
          "l": false,
          "o": false,
          "z": false,
          "i": false,
          "d": false,
          "k": false,
          "r": false,
          "y": false,
          "v": false,
          "h": false,
          "PELO MENOS UM": false,
        }
      
        for (var a; a<80; i++) targets[a].was_selected = false  // just to reset the color of all targets (because another attempt started)
        
        continue_button = createButton('START 2ND ATTEMPT');
        continue_button.mouseReleased(continueTest);
        continue_button.position(width/2 - continue_button.size().width/2, height/2 - continue_button.size().height/2);
      }
    }
    // Check if this was the first selection in an attempt
    else if (current_trial === 1) testStartTime = millis(); 
  }
}


// Evoked after the user starts its second (and last) attempt
function continueTest()
{
  // Re-randomize the trial order
  randomizeTrials();
  
  // Resets performance variables
  hits = 0;
  misses = 0;
  
  current_trial = 0;
  continue_button.remove();

  for (var i = 0; i < 80; i++) { targets[i].was_selected = false; }
  
  // Shows the targets again
  draw_targets = true; 
}


// Creates and positions the UI targets
function createTargets(target_size)
{
  vid_button.remove()

  // Sort targets by alphabetical order
  let Rows = legendas.getRows()
  let cities = []
  for (var i = 0; i < Rows.length; i++) {
    var row = Rows[i]
    cities.push({
      target_label: row.getString('city'),
      target_id: row.getNum('id')
    })
  }
  cities.sort(compareCidades)

  let counter_dic = {
    'a': 0,
    'g': 0,
    'n': 0,
    '¡': 0,
    'e': 0,
    's': 0,
    't': 0,
    'm': 0,
    'u': 0,
    'l': 0,
    'o': 0,
    'z': 0,
    'i': 0,
    'd': 0,
    'k': 0,
    'r': 0,
    'y': 0,
    'v': 0,
    'h': 0
  }

  let space_between_targets = target_size / 4
  let first_target_x = windowWidth/2 - (target_size * 7 + space_between_targets*6)/2;
  let first_target_y = 20 + target_size * 3

  for (var i = 0; i < 80; i++) {
    let counter = counter_dic[removeAccents(cities[i].target_label[cities[i].target_label.length - 1].toLowerCase())]

    let target_x = first_target_x + (counter % 7) * (target_size + space_between_targets);

    let target_y;

    if (removeAccents(cities[i].target_label[cities[i].target_label.length - 1].toLowerCase()) != 'a' && removeAccents(cities[i].target_label[cities[i].target_label.length - 1].toLowerCase()) != 'o' ) {
        target_y = first_target_y + (Math.floor(7 / 7)) * (target_size + space_between_targets);
    }
    else {
        target_y = first_target_y + (Math.floor(counter / 7)) * (target_size + space_between_targets);
    }


    targets.push(new Target(target_x, target_y, target_size, target_size, cities[i].target_label, cities[i].target_id, selected_options));
    counter_dic[removeAccents(cities[i].target_label[cities[i].target_label.length - 1].toLowerCase())]++
  }
}


function createMenuTargets(menu_target_size) {
  const suffix = ['a', 'g', 'n', 'e', 's', 't', 'm', 'u', 'l', 'o', 'z', 'i', 'd', 'k', 'r', 'y', 'v', 'h']
  suffix.sort()

  //targets centradas no meio do ecrã
  let space_between_targets = menu_target_size / 4

  let target_x = windowWidth/2 - (menu_target_size * 9 + space_between_targets*8)/2;
  let target_y = windowHeight - (menu_target_size * 2 + space_between_targets*2) - 20;
  
  for (var i = 0; i < 18; i++) {
    if (i % 9 == 0) {
      target_x = windowWidth/2 - (menu_target_size * 9 + space_between_targets*8)/2;
      if (i != 0) {
        target_y += space_between_targets + menu_target_size;
      }
    } else {
      target_x += space_between_targets + menu_target_size
    }

    let target = new Menu_Target(target_x, target_y, menu_target_size, menu_target_size, suffix[i], selected_options);
    menu_targets.push(target);
  }
}
// Is invoked when the canvas is resized (e.g., when we go fullscreen)
function windowResized() 
{
  if (fullscreen())
  {
    resizeCanvas(windowWidth, windowHeight);
    
    // DO NOT CHANGE THE NEXT THREE LINES!
    let display        = new Display({ diagonal: display_size }, window.screen);
    PPI                = display.ppi;                      // calculates pixels per inch
    PPCM               = PPI / 2.54;                       // calculates pixels per cm
  
    // Make your decisions in 'cm', so that targets have the same size for all participants
    // Below we find out out white space we can have between 2 cm targets
    let target_size    = 2; // sets the target size (will be converted to cm when passed to createTargets)
    let menu_target_size = 1.4;

    createMenuTargets(menu_target_size * PPCM);

    // Creates and positions the UI targets according to the white space defined above (in cm!)
    // 80 represent some margins around the display (e.g., for text)
    createTargets(target_size * PPCM);

    // Starts drawing targets immediately after we go fullscreen
    draw_targets = true;
  }
}


function compareCidades(a, b) {
  const nameA = a.target_label.toUpperCase()
  const nameB = b.target_label.toUpperCase()
  
  return nameA.localeCompare(nameB, 'en')
}


function removeAccents(str) {
  return str.normalize('NFD').replace(/[\u0300-\u036f]/g, '');
}


function openInNewTab() {
    window.open("tutorial.mp4", '_blank').focus();
}