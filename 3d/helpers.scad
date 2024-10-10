module cylinder_vert(h, r) {
    rotate([90, 0, 0])
        cylinder(h=h, r=r, center=true);
}

module cylinder_hollow(h, ri, ro, center) {
    difference() {
        cylinder(h=h, r=ro, center=center);
        translate([0, 0, -0.5])
          cylinder(h=h+1, r=ri, center=center);
    }
}

module half_cube(diag, l, h) {
    side = diag / sqrt(2);
    
    difference() {
        cube([side, l, side]);
        
        translate([0, -1, 0]) {
            rotate([0, -45, 0])
                cube([diag, l+2, diag/2+0.001]);
                
            rotate([0, -45, 0])
            translate([0, 0, -diag/2-h])
                cube([diag, l+2, diag/2+0.001]);
        }
    }
    
    
} 
