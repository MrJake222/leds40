include <helpers.scad>
include <screw.scad>
$fn=$preview ? 16 : 128;

x=56.5;
y=50.0;
z=14; // height (bottom board - bottom top)

W=2; // wall thickness
G=0.4; // wall gap
E=4; // elements bottom gap

// print indexes
TOP=0; BTM=1;

mx = [6, 6,     x-6, x-6];
my = [6, y-5.5, y-6, 6  ];

module mounts() {
    for (i = [0:3])
    translate([mx[i], my[i], -E]) {
        screw_hole_3x12(W, rot=180);
        //color("blue") cylinder(h=15, r=S3x12_D_th/2);
    }
}

module fins(n) {
    for (i = [0:4])
        translate([i*2, 0, 0])
        cube([1, 10.8, 10]);
}

module jack() { translate([33,   35.8,  3.5])    cube([10, 15.4+10, 10]); }
module ark()  { translate([45.5, 16.48, W])      cube([8+10,  20.5, 5]); }
module ascr() { translate([47, 17, 0])           cube([x+G-47, 19.5, 30]); }
module led()  { translate([45.35, 11.85, 0])     cylinder(r=2.6, h=15+10);  }
module ldo()  { translate([12.4-1, 38.1-1, -10]) fins(); }

module atxt() {
    translate([x+G+W-0.5, 17, 8])
    rotate([90, 0, 90])
    linear_extrude(1)
    text("B  R  G  12V", 2.5);
}

module logo() {
    translate([2.5, y-10, z+2-0.5])
    linear_extrude(1) {
        text("Leds 4.0", 10);
        
        translate([7, -11, 0])
            text("mrwski.eu", 5);
    }
}
//color("green") logo();

module board() {
    color("red") {
        // this is the board that must fit
        cube([x, y, 2]);
        mounts();
        jack();
        ark();
        led();
        ascr(); // ark screws
        atxt(); // ark text
        ldo();
        logo();
    }
}

module ear(rot) {
    R=2.2;
    rotate(rot)
    translate([-10, -5, 0])
    difference() {
        linear_extrude(W)
            polygon(points=[[0,0],[3.5,10],[10-R,10],[10-R,5.5],[10+R,5.5],[10+R,10],[16.5,10],[20,0]]);
        translate([10, 5, -0.1])
            cylinder(r1=R, r2=4, h=W+0.2);
    }
}

module case(print) {
    o=W+G;  // side offset
    bo=E-G; // bottom offset
    if (print==TOP) {
        translate([-o,  -o,   z]) cube([o+x+o, o+y+o, W]); // top
        translate([-o,  -o,  -bo]) cube([o+x+o, W, bo+z]); // front
        translate([x+G, -o,  -bo]) cube([W, o+y+o, bo+z]); // right
        translate([-o,  -o,  -bo]) cube([W, o+y+o, bo+z]); // left
        translate([-o,  y+G, -bo]) cube([o+x+o, W, bo+z]); // back
    }
    else {
        color("gray") {
            translate([-o, -o, -(W+E)]) cube([o+x+o, o+y+o, W]); // bottom
            translate([-o-5,   y/2, -(W+E)]) ear(90);
            translate([ o+5+x, y/2, -(W+E)]) ear(-90);
        }
        
        // pilot, size of the board
        w=0.5;
        h=E/2;
        color("cyan") {
            translate([0,   0,   -E]) cube([x, w, h]); // front
            translate([x-w, 0,   -E]) cube([w, y, h]); // right
            translate([0,   0,   -E]) cube([w, y, h]); // left
            translate([0,   y-w, -E]) cube([x, w, h]); // back
        }
    }
    
    // screws
    for (i = [0:3])
    translate([mx[i], my[i], 0]) {
        // long sockets
        if (print==TOP) translate([0, 0, W]) screw_socket(S3x12_D, S3x12_W, z, round=true);
        // supports
        else translate([0, 0, -E]) cylinder_hollow(h=E, ri=S3x12_D_th/2, ro=4.5);
    }
}

module merge(print) {
    difference() {
        case(print);
        board();
    }
}
//board();

rotate([180, 0, 0]) merge(TOP);
//merge(BTM);
//merge(TOP);
