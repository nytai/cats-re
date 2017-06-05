external gatherImages : string => array Js.Json.t = "" [@@bs.module "js/helpers"];

open Bs_fetch;

module Option = {
  let unwrapUnsafely =
    fun
    | Some v => v
    | None => raise (Invalid_argument "unwrapUnsafely called on None");
};

let getPics _ =>
  Js.Promise.(
    fetch "http://mapd-cats.azurewebsites.net/catpics" |> then_ Response.text |>
    then_ (fun xml => gatherImages xml |> resolve)
  );

let getFacts _ =>
  Js.Promise.(
    fetch "http://mapd-cats.azurewebsites.net/catfacts" |> then_ Response.json |>
    then_ (
      fun json =>
        Js.Json.decodeObject json |> Option.unwrapUnsafely |> (fun d => Js.Dict.get d "facts") |> Option.unwrapUnsafely |> Js.Json.decodeArray |> Option.unwrapUnsafely |> resolve
    )
  );

let zip xs ys => Array.init (Array.length xs) (fun i => (xs.(i), ys.(i)));

let getCats _ =>
  Js.Promise.(
    all [|getPics (), getFacts ()|] |>
    then_ (
      fun [|images, facts|] =>
        zip images facts |>
        Array.mapi (
          fun i (image, fact) => {
            let image = Js.Json.decodeString image |> Option.unwrapUnsafely;
            let fact = Js.Json.decodeString fact |> Option.unwrapUnsafely;
            ({id: i, image, fact, isHidden: false}: Cat.cat)
          }
        ) |> Array.to_list |> resolve
    )
  );
