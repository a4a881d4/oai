time $OPENAIR_TARGETS/SIMU/USER/oaisim -X1 -i0 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X1 -i1 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X1 -i2 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X1 -i1 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X2 -i0 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X2 -i1 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X2 -i2 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X2 -i3 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X2 -i4 -n200 >/dev/null &
time $OPENAIR_TARGETS/SIMU/USER/oaisim -X2 -i5 -n200 
#time $OPENAIR_TARGETS/SIMU/USER/oaisim -X2 -i6 -n200 >/dev/null &
#time $OPENAIR_TARGETS/SIMU/USER/oaisim -X2 -i7 -n200 >/dev/null &

#time $OPENAIR_TARGETS/SIMU/USER/oaisim -X0 -n200

#valgrind --tool=callgrind --collect-bus=yes --branch-sim=yes ./oaisim -X0 -n5