module screw_socket(d, w, h, round=false) {
    // d -- inner screw hole diameter
    // w -- wall width
    // h -- height of socket
    
    // outer cube side
    a = d + 2*w;
    
    translate([0, 0, h/2])
    difference() {
      if (round) cylinder(r=a/2, h=h, center=true);
      else cube([a, a, h], center=true);
    
      color("red")
        cylinder(r=d/2, h=h+1, center=true);
    }
}

module screw_hole(d1, d2, h, rot=0) {
  // d1 -- bottom, smaller diameter (screw passes through it)
  // d2 -- top, larger diameter (screw head stops on it)
  // h -- height of wall to penetrate
  // rot -- rotation
  
  rotate([rot, 0, 0])
    cylinder(r1=d1/2, r2=d2/2, h=h);
}

S3x12_D = 2.8;
S3x12_D_th = 3.3; // through
S3x12_D_top = 5.7+0.5; // top head
S3x12_W = 2;
S3x12_H = 11;
module screw_socket_3x12(round=false) {
  screw_socket(S3x12_D, S3x12_W, S3x12_H, round);
}
module screw_hole_3x12(h, rot=0) {
  // h -- wall width
  // rot -- rotation
  screw_hole(S3x12_D_th, S3x12_D_top, h=h+0.001, rot=rot);
}
