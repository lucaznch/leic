// Bake-off #2 -- Seleção em Interfaces Densas
// IPM 2023-24, Período 3
// Entrega: até às 23h59, dois dias úteis antes do sexto lab (via Fenix)
// Bake-off: durante os laboratórios da semana de 18 de Março

// p5.js reference: https://p5js.org/reference/

// Database (CHANGE THESE!)
const GROUP_NUMBER        = 0;      // Add your group number here as an integer (e.g., 2, 3)
const RECORD_TO_FIREBASE  = false;  // Set to 'true' to record user results to Firebase

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
  "Ba": false,
  "Br": false,
  "Be": false,
  "Bu": false,
  "Bi": false,
  "Bo": false,
  "Bh": false,
  "By": false,
  "Bl": false,
  "Bn": false,
  "PELO MENOS UM": false,
}

const GRID_ROWS           = 11;      // We divide our 80 targets in a 8x10 grid
const GRID_COLUMNS        = 14;     // We divide our 80 targets in a 8x10 grid

let ignored_size = Array.from({length: 8}, (_, i) => i + 1);

// Ensures important data is loaded before the program starts
function preload()
{
  // id,name,...
  legendas = loadTable('legendas.csv', 'csv', 'header');
}

// Runs once at the start
function setup()
{
  createCanvas(700, 500);    // window size in px before we go into fullScreen()
  frameRate(60);             // frame rate (DO NOT CHANGE!)
  
  randomizeTrials();         // randomize the trial order at the start of execution
  drawUserIDScreen();        // draws the user start-up screen (student ID and display size)
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
    for (var i = 0; i < 10; i++) menu_targets[i].draw();

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
        targets[i].unselect() // DONT CALL ME!!!!
        break;
      }
    }
    
    for (var j = 0; j < 10; j++) {
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
      {
        selected_options = { // Reset selected options (for the menu targets)
          "Ba": false,
          "Br": false,
          "Be": false,
          "Bu": false,
          "Bi": false,
          "Bo": false,
          "Bh": false,
          "By": false,
          "Bl": false,
          "Bn": false,
          "PELO MENOS UM": false,
        }
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
function createTargets(target_size, vertical_gap)
{
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

  let counter = 0;
  let pref_counter = 0;
  let last_prefix =  removeAccents("Ba")
  
  let vert_space_between_targets = vertical_gap / (4 - 1)
  let hor_space_between_targets = target_size / 4

  let target_x = windowWidth/2 - (target_size * 7 + hor_space_between_targets * 6) / 2;
  let target_y = (40 + target_size)
  
  while (counter < 80) {
    let current_prefix = removeAccents(cities[counter].target_label.substring(0,2))

    
    if (current_prefix != last_prefix) {
      pref_counter = 0
      last_prefix = current_prefix
      target_x = windowWidth/2 - (target_size * 7 + hor_space_between_targets * 6) / 2;
      
      if (current_prefix == "Bh" || current_prefix == "Bl" || current_prefix == "Bn" || current_prefix == "Bo" || current_prefix == "By") { target_y = 4*(20 + target_size); }

      else { target_y = 3*(20 + target_size); }
    } 
    
    else if (pref_counter % 7 == 0) {
      target_x = windowWidth/2 - (target_size * 7 + hor_space_between_targets * 6) / 2;
      if (pref_counter != 0) {
        target_y += target_size + 10
      }
    } else {
      target_x += target_size + hor_space_between_targets
    }

    targets.push(new Target(target_x, target_y, target_size, target_size, cities[counter].target_label, cities[counter].target_id, selected_options));

    counter++;
    pref_counter++;
  }
}

function createMenuTargets(menu_target_size) {
  const prefix = ['Ba', 'Br', 'Be', 'Bu', 'Bi', 'Bo', 'Bh', 'By', 'Bl', 'Bn']
  prefix.sort()

  //targets centradas no meio do ecrã
  let space_between_targets = menu_target_size / 4

  let target_x = windowWidth/2 - (menu_target_size * 2.5 + space_between_targets*2);
  let target_y = windowHeight - (menu_target_size * 2 + space_between_targets*2) - 20;
  
  for (var i = 0; i < 10; i++) {
    if (i % 5 == 0) {
      target_x = windowWidth/2 - (menu_target_size * 2.5 + space_between_targets*2);
      if (i != 0) {
        target_y += space_between_targets + menu_target_size;
      }
    } else {
      target_x += space_between_targets + menu_target_size
    }

    let target = new Menu_Target(target_x, target_y, menu_target_size, menu_target_size, prefix[i], selected_options);
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
    let screen_width   = display.width * 2.54;             // screen width
    let screen_height  = display.height * 2.54;            // screen height
    let target_size    = 1.8;                                // sets the target size (will be converted to cm when passed to createTargets)
    let target_vert_size = 1.8;
    let target_size_by_letter = 0.220
    let menu_target_size = 1.9;

    let horizontal_gap = screen_width - target_size * 7;
    let vertical_gap   = screen_height - ((menu_target_size * 2 + (menu_target_size / 4) *2) + 20) - target_vert_size; // 20 for the bar at the bottom

    createMenuTargets(menu_target_size * PPCM);

    // Creates and positions the UI targets according to the white space defined above (in cm!)
    // 80 represent some margins around the display (e.g., for text)
    createTargets(target_size * PPCM, vertical_gap * PPCM - 80);

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