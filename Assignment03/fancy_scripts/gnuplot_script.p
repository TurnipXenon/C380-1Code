#filename="test"
png_ext='.png'
txt_ext='.txt'
full=filename.txt_ext
set terminal png size 900,400
set datafile separator ","
#unset autoscale
#set autoscale xfix
set yrange[0:]
set xtics nomirror
set xlabel "Time (memory access)"
set ylabel "Windowset size"
set output filename.png_ext
set title full
plot full w linesp
#plot full