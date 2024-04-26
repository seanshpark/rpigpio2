OUTPUT=$1

_all_on()
{
  ${OUTPUT}/pinctrl set 23 op dh
  ${OUTPUT}/pinctrl set 24 op dh
  sleep 1
}

_all_off()
{
  ${OUTPUT}/pinctrl set 23 op dl
  ${OUTPUT}/pinctrl set 24 op dl
  sleep 1
}

_toggle_1()
{
  ${OUTPUT}/pinctrl set 23 op dh
  ${OUTPUT}/pinctrl set 24 op dl
  sleep 0.2
}

_toggle_2()
{
  ${OUTPUT}/pinctrl set 23 op dl
  ${OUTPUT}/pinctrl set 24 op dh
  sleep 0.2
}

_all_on
_all_off

loopcnt=0
while [[ "$loopcnt" -lt 5 ]]
do
  _toggle_1
  _toggle_2
  loopcnt=$(expr $loopcnt + 1)
done

_all_off
