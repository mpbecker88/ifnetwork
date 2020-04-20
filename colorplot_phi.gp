set terminal pngcairo  enhanced font "arial,14" fontscale 1.0 size 1800, 900 

set output 'phi05.png'

set multiplot layout 2,3 rowsfirst

set title '{/Symbol a}=0.2'
set xlabel ''
set ylabel 'g^{EI}'
set cblabel ''

set xrange [1:4]
set yrange [1:4]
set cbrange [0:5]

plot 'phi05-02.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=0.4'
set xlabel ''
set ylabel ''
set cblabel ''

set cbrange [0:5]

plot 'phi05-04.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=0.6'
set xlabel ''
set ylabel ''
set cblabel ''

set cbrange [0:5]

plot 'phi05-06.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=0.8'
set xlabel 'g^{IE}'
set ylabel 'g^{EI}'
set cblabel ''

set cbrange [0:5]

plot 'phi05-08.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=1.0'
set xlabel 'g^{IE}'
set ylabel ''
set cblabel ''

set cbrange [0:5]

plot 'phi05-10.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=random'
set xlabel 'g^{IE}'
set ylabel ''
set cblabel ''

set cbrange [0:5]

plot 'phi05-r.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

unset multiplot



set output 'phi10.png'

set multiplot layout 2,3 rowsfirst

set title '{/Symbol a}=0.2'
set xlabel ''
set ylabel 'g^{EI}'
set cblabel ''

set cbrange [0:5]

plot 'phi10-02.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=0.4'
set xlabel ''
set ylabel ''
set cblabel ''

set cbrange [0:5]

plot 'phi10-04.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=0.6'
set xlabel ''
set ylabel ''
set cblabel ''

set cbrange [0:5]

plot 'phi10-06.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=0.8'
set xlabel 'g^{IE}'
set ylabel 'g^{EI}'
set cblabel ''

set cbrange [0:5]

plot 'phi10-08.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=1.0'
set xlabel 'g^{IE}'
set ylabel ''
set cblabel ''

set cbrange [0:5]

plot 'phi10-10.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

set title '{/Symbol a}=random'
set xlabel 'g^{IE}'
set ylabel ''
set cblabel ''

set cbrange [0:5]

plot 'phi10-r.dat' u (($1)*.25 + 1):(($2)*.25+1):3 matrix w image t ''

unset multiplot