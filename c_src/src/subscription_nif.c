#ifdef __cplusplus
extern "C"
{
#endif
#include <erl_nif.h>
#include "../include/total_nif.h"
#include "../include/subscription_nif.h"
#include <stdio.h>
#include <string.h>

#include "rclex/subsription.h"

ERL_NIF_TERM nif_rcl_get_zero_initialized_subscription(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 0){
      return enif_make_badarg(env);
    }
    rcl_subscription_t* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_sub,sizeof(rcl_subscription_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    *res = rcl_get_zero_initialized_subscription();

    return ret;
}

ERL_NIF_TERM nif_rcl_subscription_get_default_options(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
  if(argc != 0){
    return enif_make_badarg(env);
  }
  rcl_subscription_options_t* res;
  ERL_NIF_TERM ret;
  res = enif_alloc_resource(rt_sub_options,sizeof(rcl_subscription_options_t));
  if(res == NULL) return enif_make_badarg(env);
  ret = enif_make_resource(env,res);
  enif_release_resource(res);
  *res = rcl_subscription_get_default_options();
  return ret;
}

/*
  rcl_ret_t
  rcl_subscription_init(
    rcl_subscription_t * subscription,
    const rcl_node_t * node,
    const rosidl_message_type_support_t * type_support,
    const char * topic_name,
    const rcl_subscription_options_t * options
  );
*/
ERL_NIF_TERM nif_rcl_subscription_init(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
  if(argc != 5){
    return enif_make_badarg(env);
  }
  rcl_ret_t* res_ret;
  ERL_NIF_TERM ret;
  rcl_subscription_t*  res_sub;
  rcl_node_t* res_node;
  rosidl_message_type_support_t* res_idl;
  rcl_subscription_options_t* res_sub_options;
  
  if(!enif_get_resource(env, argv[0], rt_sub, (void**) &res_sub)){
    return enif_make_badarg(env);
  }
  
  if(!enif_get_resource(env, argv[1], rt_node, (void**) &res_node)){
    return enif_make_badarg(env);
  }
  if(!enif_get_resource(env, argv[2], rt_rosidl_msg_type_support, (void**) &res_idl)){
    return enif_make_badarg(env);
  }
  char topic_buf[128]; //トピック名を格納するためのバッファ
  (void)memset(&topic_buf,'\0',sizeof(topic_buf));
  if(!enif_get_string(env,argv[3],topic_buf,sizeof(topic_buf),ERL_NIF_LATIN1)){
    return enif_make_badarg(env);
  }
  
  if(!enif_get_resource(env, argv[4], rt_sub_options, (void**) &res_sub_options)){
    return enif_make_badarg(env);
  }
  
  //返すrcl_ret_tについて，alloc_resource
  res_ret = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
  if(res_ret == NULL) return enif_make_badarg(env);
  
  ret = enif_make_resource(env,res_ret);
  enif_release_resource(res_ret);
  *res_ret = rcl_subscription_init(res_sub,res_node,res_idl,topic_buf,res_sub_options);
  printf("exit publisher_init\n");
  return ret;
}

/*
  rcl_ret_t
  rcl_subscription_fini(
    rcl_subscription_t * subscription, 
    rcl_node_t * node
  );
*/

ERL_NIF_TERM nif_rcl_subscription_fini(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
  rcl_ret_t* res;
  ERL_NIF_TERM ret;

  rcl_subscription_t* res_sub;
  rcl_node_t* res_node;

  if(argc != 2){
      return enif_make_badarg(env);
  }
  if(!enif_get_resource(env, argv[0], rt_sub, (void**) &res_sub)){
    return enif_make_badarg(env);
  }
  if(!enif_get_resource(env, argv[1], rt_node, (void**) &res_node)){
    return enif_make_badarg(env);
  }

  res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
  if(res == NULL) return enif_make_badarg(env);
  ret = enif_make_resource(env,res);
  enif_release_resource(res);
  
  *res = rcl_subscription_fini(res_sub,res_node);
  
  return ret;
} 
/*
  rcl_ret_t
  rcl_take(
    const rcl_subscription_t * subscription,
    void * ros_message,
    rmw_message_info_t * message_info,
    rmw_subscription_allocation_t * allocation
  );
*/
ERL_NIF_TERM nif_rcl_take(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
  rcl_ret_t* res;
  ERL_NIF_TERM ret;

  rcl_subscription_t* res_sub;
  rmw_message_info_t* res_msginfo;  
  rmw_subscription_allocation_t* res_sub_alloc;
  //上2つはinclude/rmw/types.hに定義されてる
  int a;
  const void * ros_message; //void*にはどんな型でも入って，使う場合に任意の型にキャストする．
  //rmw_publisher_allocation_t* res_arg_puballoc; #一旦NULLで済ます
  
  if(argc != 4){
      return enif_make_badarg(env);
  }
  if(!enif_get_resource(env, argv[0], rt_sub, (void**) &res_sub)){
    return enif_make_badarg(env);
  }

  if(!enif_get_int(env,argv[1],&a)){
        return enif_make_badarg(env);
    }
  
  
  if(!enif_get_resource(env, argv[2], rt_, (void**) &res_msginfo)){
    return enif_make_badarg(env);
  }
  if(!enif_get_resource(env, argv[3], rt_, (void**) &res_sub_alloc)){
    return enif_make_badarg(env);
  }
  res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
  if(res == NULL) return enif_make_badarg(env);
  ret = enif_make_resource(env,res);
  enif_release_resource(res);

  *res = rcl_take(res_sub,&a,res_msginfo,res_sub_alloc);
  
  return ret;
}

#ifdef __cplusplus
}
#endif