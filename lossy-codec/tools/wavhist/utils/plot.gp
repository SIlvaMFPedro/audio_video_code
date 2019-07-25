DATAFILE = "/tmp/wavhist/histogram.dat"
set output '~/.wavhist/histogram.png'
set terminal png enhanced

set grid
set key below center horizontal noreverse enhanced autotitle box dashtype solid
set tics out nomirror
set border 1 front linetype black linewidth 1.0 dashtype solid
set xtics rotate 

set style histogram 
set style data histograms

set boxwidth 1.0 absolute
set style fill solid 2.0 border 1

plot DATAFILE using 2:xtic(int($0)%1000 == 0 ? stringcolumn(1) : '') title "channel"