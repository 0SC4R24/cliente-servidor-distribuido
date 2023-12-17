# Ejecutar el binario dependiendo del parametro que se le pase
case $1 in
mm_server)
    /practicas/mm_server
    ;;

mm_client)
    /practicas/mm_client
    ;;

fm_server)
    /practicas/fm_server
    ;;

fm_client)
    /practicas/fm_client
    ;;

broker)
    /practicas/broker
    ;;

*)
  echo "No se ha encontrado el binario"
  ;;
esac