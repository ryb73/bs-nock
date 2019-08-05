type t;
type scope;

[@bs.module "nock"] external cleanAll: unit => unit = "";
[@bs.module "nock"] external disableNetConnect: unit => unit = "";
[@bs.module "nock"] external isDone: unit => bool = "";
[@bs.module "nock"] external pendingMocks: unit => array(string) = "";

[@bs.module] external nock: string => t = "";

[@bs.send.pipe: t] external get: string => scope = "";
[@bs.send.pipe: t] external post: string => scope = "";

[@bs.send.pipe: scope] external reply: (int, ~response: string=?, unit) => unit = "";
let reply = (~response=?, code) =>
reply(code, ~response=?Belt.Option.map(response, Js.Json.stringify), ());

[@bs.send.pipe: scope] external replyObj: (int, Js.t({..})) => unit = "reply";
